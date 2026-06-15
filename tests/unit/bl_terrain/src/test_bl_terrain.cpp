#include <cstddef>
#include <cstdio>

#include "bl_terrain.h"

namespace {

// Terrain fixtures
constexpr char kSingleAgentTerrain[] = "A";
constexpr char kSingleTargetTerrain[] = "T";
constexpr char kSingleBlockedTerrain[] = "#";
constexpr char kSingleUnblockedTerrain[] = ".";

constexpr char kFirstAgentTerrain[] = "A..#..T.";
constexpr char kMidAgentTerrain[] = "..A#..T.";
constexpr char kLastAgentTerrain[] = "...#..TA";
constexpr char kMultipleAgentTerrain[] = "..A#.A.T";
constexpr char kNoAgentTerrain[] = "...#..T.";

constexpr char kFirstTargetTerrain[] = "T..#..A.";
constexpr char kMidTargetTerrain[] = "..T#..A.";
constexpr char kLastTargetTerrain[] = "...#..AT";
constexpr char kMultipleTargetTerrain[] = "..T#.T.A";
constexpr char kNoTargetTerrain[] = "A..#....";

constexpr char kSimpleBlockedTerrain[] = "A...#..T";

// Assertion helpers
int expect_status(const char *label, bl_status_t actual,
                  bl_status_t expected) noexcept {
  if (actual != expected) {
    std::printf("%s status: expected %d, got %d\n", label, expected, actual);
    return 1;
  }

  return 0;
}

int expect_index(const char *label, std::size_t actual,
                 std::size_t expected) noexcept {
  if (actual != expected) {
    std::printf("%s index: expected %zu, got %zu\n", label, expected, actual);
    return 1;
  }

  return 0;
}

int expect_is_blocked(const char *label, bool actual, bool expected) noexcept {
  if (actual != expected) {
    std::printf("%s is_blocked: %s", label,
                actual ? "expected false, got true\n"
                       : "expected true, got false\n");
    return 1;
  }

  return 0;
}

// Agent lookup success cases
int test_terrain_locate_agent_1d_present_single_cell_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kSingleAgentTerrain, sizeof(kSingleAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate agent (present, single cell)", status, BL_OK);
  failures += expect_index("locate agent (present, single cell)", idx, 0U);

  return failures;
}

int test_terrain_locate_agent_1d_present_first_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kFirstAgentTerrain, sizeof(kFirstAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (first position)", status, BL_OK);
  failures += expect_index("locate agent (first position)", idx, 0U);

  return failures;
}

int test_terrain_locate_agent_1d_present_mid_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kMidAgentTerrain, sizeof(kMidAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (mid position)", status, BL_OK);
  failures += expect_index("locate agent (mid position)", idx, 2U);

  return failures;
}

int test_terrain_locate_agent_1d_present_last_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kLastAgentTerrain, sizeof(kLastAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (last position)", status, BL_OK);
  failures += expect_index("locate agent (last position)", idx, 7U);

  return failures;
}

int test_terrain_locate_agent_1d_present_multiple_returns_bl_ok_and_first_found_idx() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kMultipleAgentTerrain, sizeof(kMultipleAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (multiple)", status, BL_OK);
  failures += expect_index("locate agent (multiple)", idx, 2U);

  return failures;
}

// Agent lookup not-found and length-boundary cases
int test_terrain_locate_agent_1d_missing_single_cell_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kSingleTargetTerrain, sizeof(kSingleTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (missing, single cell)", status,
                            BL_ERR_NOT_FOUND);

  return failures;
}

int test_terrain_locate_agent_1d_missing_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kNoAgentTerrain, sizeof(kNoAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (missing)", status, BL_ERR_NOT_FOUND);

  return failures;
}

int test_terrain_locate_agent_1d_missing_leaves_index_unchanged() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d(
      kNoAgentTerrain, sizeof(kNoAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (missing, unchanged index)", status,
                            BL_ERR_NOT_FOUND);
  failures += expect_index("locate agent (missing, unchanged index)", idx, 99U);

  return failures;
}

int test_terrain_locate_agent_1d_present_after_length_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status =
      bl_terrain_locate_agent_1d(kLastAgentTerrain, 4U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate agent (after length)", status, BL_ERR_NOT_FOUND);

  return failures;
}

// Agent lookup argument validation
int test_terrain_locate_agent_1d_null_terrain_returns_bl_err_null() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status =
      bl_terrain_locate_agent_1d(nullptr, sizeof(kMidAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate agent (null terrain)", status, BL_ERR_NULL);

  return failures;
}

int test_terrain_locate_agent_1d_zero_length_returns_bl_err_invalid_arg() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_agent_1d("", 0U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate agent (zero length)", status, BL_ERR_INVALID_ARG);

  return failures;
}

int test_terrain_locate_agent_1d_oversized_length_returns_bl_err_bounds() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status =
      bl_terrain_locate_agent_1d(kMidAgentTerrain, BL_MAX_1D_CELLS + 1, &idx);

  int failures = 0;
  failures +=
      expect_status("locate agent (oversized length)", status, BL_ERR_BOUNDS);

  return failures;
}

int test_terrain_locate_agent_1d_null_index_returns_bl_err_null() noexcept {
  const bl_status_t status = bl_terrain_locate_agent_1d(
      kMidAgentTerrain, sizeof(kMidAgentTerrain) - 1U, nullptr);

  int failures = 0;
  failures += expect_status("locate agent (null index)", status, BL_ERR_NULL);

  return failures;
}

// Target lookup success cases
int test_terrain_locate_target_1d_present_single_cell_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kSingleTargetTerrain, sizeof(kSingleTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (present, single cell)", status, BL_OK);
  failures += expect_index("locate target (present, single cell)", idx, 0U);

  return failures;
}

int test_terrain_locate_target_1d_present_first_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kFirstTargetTerrain, sizeof(kFirstTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (first position)", status, BL_OK);
  failures += expect_index("locate target (first position)", idx, 0U);

  return failures;
}

int test_terrain_locate_target_1d_present_mid_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kMidTargetTerrain, sizeof(kMidTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (mid position)", status, BL_OK);
  failures += expect_index("locate target (mid position)", idx, 2U);

  return failures;
}

int test_terrain_locate_target_1d_present_last_returns_bl_ok_and_index() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kLastTargetTerrain, sizeof(kLastTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (last position)", status, BL_OK);
  failures += expect_index("locate target (last position)", idx, 7U);

  return failures;
}

int test_terrain_locate_target_1d_present_multiple_returns_bl_ok_and_first_found_idx() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kMultipleTargetTerrain, sizeof(kMultipleTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (multiple)", status, BL_OK);
  failures += expect_index("locate target (multiple)", idx, 2U);

  return failures;
}

// Target lookup not-found and length-boundary cases
int test_terrain_locate_target_1d_missing_single_cell_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kSingleAgentTerrain, sizeof(kSingleAgentTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (missing, single cell)", status,
                            BL_ERR_NOT_FOUND);

  return failures;
}

int test_terrain_locate_target_1d_missing_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kNoTargetTerrain, sizeof(kNoTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (missing)", status, BL_ERR_NOT_FOUND);

  return failures;
}

int test_terrain_locate_target_1d_missing_leaves_index_unchanged() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      kNoTargetTerrain, sizeof(kNoTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures += expect_status("locate target (missing, unchanged index)", status,
                            BL_ERR_NOT_FOUND);
  failures +=
      expect_index("locate target (missing, unchanged index)", idx, 99U);

  return failures;
}

int test_terrain_locate_target_1d_present_after_length_returns_bl_err_not_found() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status =
      bl_terrain_locate_target_1d(kLastTargetTerrain, 4U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (after length)", status, BL_ERR_NOT_FOUND);

  return failures;
}

// Target lookup argument validation
int test_terrain_locate_target_1d_null_terrain_returns_bl_err_null() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d(
      nullptr, sizeof(kMidTargetTerrain) - 1U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (null terrain)", status, BL_ERR_NULL);

  return failures;
}

int test_terrain_locate_target_1d_zero_length_returns_bl_err_invalid_arg() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status = bl_terrain_locate_target_1d("", 0U, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (zero length)", status, BL_ERR_INVALID_ARG);

  return failures;
}

int test_terrain_locate_target_1d_oversized_length_returns_bl_err_bounds() noexcept {
  std::size_t idx = 99U;

  const bl_status_t status =
      bl_terrain_locate_target_1d(kMidTargetTerrain, BL_MAX_1D_CELLS + 1, &idx);

  int failures = 0;
  failures +=
      expect_status("locate target (oversized length)", status, BL_ERR_BOUNDS);

  return failures;
}

int test_terrain_locate_target_1d_null_index_returns_bl_err_null() noexcept {
  const bl_status_t status = bl_terrain_locate_target_1d(
      kMidTargetTerrain, sizeof(kMidTargetTerrain) - 1U, nullptr);

  int failures = 0;
  failures += expect_status("locate target (null index)", status, BL_ERR_NULL);

  return failures;
}

// Is blocked success cases
int test_terrain_is_blocked_1d_blocked_single_cell_returns_bl_ok_and_true() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, sizeof(kSingleBlockedTerrain) - 1, 0U,
      &is_blocked);

  int failures = 0;
  failures += expect_status("is blocked (blocked, single cell)", status, BL_OK);
  failures +=
      expect_is_blocked("is blocked (blocked, single cell)", is_blocked, true);

  return failures;
}

int test_terrain_is_blocked_1d_unblocked_single_cell_returns_bl_ok_and_false() noexcept {
  bool is_blocked = true;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleUnblockedTerrain, sizeof(kSingleUnblockedTerrain) - 1U, 0U,
      &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (unblocked, single cell)", status, BL_OK);
  failures += expect_is_blocked("is blocked (unblocked, single cell)",
                                is_blocked, false);

  return failures;
}

int test_terrain_is_blocked_1d_blocked_nonzero_index_returns_bl_ok_and_true() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSimpleBlockedTerrain, sizeof(kSimpleBlockedTerrain) - 1U, 4U,
      &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (blocked, nonzero index)", status, BL_OK);
  failures += expect_is_blocked("is blocked (blocked, nonzero index)",
                                is_blocked, true);

  return failures;
}

int test_terrain_is_blocked_1d_agent_cell_returns_bl_ok_and_false() noexcept {
  bool is_blocked = true;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleAgentTerrain, sizeof(kSingleAgentTerrain) - 1U, 0U, &is_blocked);

  int failures = 0;
  failures += expect_status("is blocked (agent cell)", status, BL_OK);
  failures +=
      expect_is_blocked("is blocked (agent cell)", is_blocked, false);

  return failures;
}

int test_terrain_is_blocked_1d_target_cell_returns_bl_ok_and_false() noexcept {
  bool is_blocked = true;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleTargetTerrain, sizeof(kSingleTargetTerrain) - 1U, 0U,
      &is_blocked);

  int failures = 0;
  failures += expect_status("is blocked (target cell)", status, BL_OK);
  failures +=
      expect_is_blocked("is blocked (target cell)", is_blocked, false);

  return failures;
}

// Is blocked argument validation
int test_terrain_is_blocked_1d_null_terrain_returns_bl_err_null() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      nullptr, sizeof(kSingleBlockedTerrain) - 1U, 0U, &is_blocked);

  int failures = 0;
  failures += expect_status("is blocked (null terrain)", status, BL_ERR_NULL);

  return failures;
}

int test_terrain_is_blocked_1d_zero_length_returns_bl_err_invalid_arg() noexcept {
  bool is_blocked = false;

  const bl_status_t status =
      bl_terrain_is_blocked_1d("", 0U, 0U, &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (zero length)", status, BL_ERR_INVALID_ARG);

  return failures;
}

int test_terrain_is_blocked_1d_oversized_length_returns_bl_err_bounds() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, BL_MAX_1D_CELLS + 1U, 0U, &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (oversized length)", status, BL_ERR_BOUNDS);

  return failures;
}

int test_terrain_is_blocked_1d_index_at_length_returns_bl_err_bounds() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, sizeof(kSingleBlockedTerrain) - 1U, 1U,
      &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (index at length)", status, BL_ERR_BOUNDS);

  return failures;
}

int test_terrain_is_blocked_1d_index_past_length_returns_bl_err_bounds() noexcept {
  bool is_blocked = false;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, sizeof(kSingleBlockedTerrain) - 1U, 2U,
      &is_blocked);

  int failures = 0;
  failures +=
      expect_status("is blocked (index past length)", status, BL_ERR_BOUNDS);

  return failures;
}

int test_terrain_is_blocked_1d_error_leaves_blocked_out_unchanged() noexcept {
  bool is_blocked = true;

  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, sizeof(kSingleBlockedTerrain) - 1U, 1U,
      &is_blocked);

  int failures = 0;
  failures += expect_status("is blocked (index at length, unchanged output)",
                            status, BL_ERR_BOUNDS);
  failures += expect_is_blocked("is blocked (index at length, unchanged output)",
                                is_blocked, true);

  return failures;
}

int test_terrain_is_blocked_1d_null_blocked_out_returns_bl_err_null() noexcept {
  const bl_status_t status = bl_terrain_is_blocked_1d(
      kSingleBlockedTerrain, sizeof(kSingleBlockedTerrain) - 1U, 0U, nullptr);

  int failures = 0;
  failures +=
      expect_status("is blocked (null output)", status, BL_ERR_NULL);

  return failures;
}

} // namespace

int main(void) {
  int failures = 0;

  // Agent lookup tests
  failures +=
      test_terrain_locate_agent_1d_present_single_cell_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_agent_1d_present_first_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_agent_1d_present_mid_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_agent_1d_present_last_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_agent_1d_present_multiple_returns_bl_ok_and_first_found_idx();
  failures +=
      test_terrain_locate_agent_1d_present_after_length_returns_bl_err_not_found();
  failures +=
      test_terrain_locate_agent_1d_missing_single_cell_returns_bl_err_not_found();
  failures += test_terrain_locate_agent_1d_missing_returns_bl_err_not_found();
  failures += test_terrain_locate_agent_1d_missing_leaves_index_unchanged();

  failures += test_terrain_locate_agent_1d_null_terrain_returns_bl_err_null();
  failures +=
      test_terrain_locate_agent_1d_zero_length_returns_bl_err_invalid_arg();
  failures +=
      test_terrain_locate_agent_1d_oversized_length_returns_bl_err_bounds();
  failures += test_terrain_locate_agent_1d_null_index_returns_bl_err_null();

  // Target lookup tests
  failures +=
      test_terrain_locate_target_1d_present_single_cell_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_target_1d_present_first_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_target_1d_present_mid_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_target_1d_present_last_returns_bl_ok_and_index();
  failures +=
      test_terrain_locate_target_1d_present_multiple_returns_bl_ok_and_first_found_idx();
  failures +=
      test_terrain_locate_target_1d_present_after_length_returns_bl_err_not_found();
  failures +=
      test_terrain_locate_target_1d_missing_single_cell_returns_bl_err_not_found();
  failures += test_terrain_locate_target_1d_missing_returns_bl_err_not_found();
  failures += test_terrain_locate_target_1d_missing_leaves_index_unchanged();

  failures += test_terrain_locate_target_1d_null_terrain_returns_bl_err_null();
  failures +=
      test_terrain_locate_target_1d_zero_length_returns_bl_err_invalid_arg();
  failures +=
      test_terrain_locate_target_1d_oversized_length_returns_bl_err_bounds();
  failures += test_terrain_locate_target_1d_null_index_returns_bl_err_null();

  // Is blocked tests
  failures +=
      test_terrain_is_blocked_1d_blocked_single_cell_returns_bl_ok_and_true();
  failures +=
      test_terrain_is_blocked_1d_unblocked_single_cell_returns_bl_ok_and_false();
  failures +=
      test_terrain_is_blocked_1d_blocked_nonzero_index_returns_bl_ok_and_true();
  failures += test_terrain_is_blocked_1d_agent_cell_returns_bl_ok_and_false();
  failures += test_terrain_is_blocked_1d_target_cell_returns_bl_ok_and_false();

  failures += test_terrain_is_blocked_1d_null_terrain_returns_bl_err_null();
  failures +=
      test_terrain_is_blocked_1d_zero_length_returns_bl_err_invalid_arg();
  failures +=
      test_terrain_is_blocked_1d_oversized_length_returns_bl_err_bounds();
  failures += test_terrain_is_blocked_1d_index_at_length_returns_bl_err_bounds();
  failures +=
      test_terrain_is_blocked_1d_index_past_length_returns_bl_err_bounds();
  failures +=
      test_terrain_is_blocked_1d_error_leaves_blocked_out_unchanged();
  failures +=
      test_terrain_is_blocked_1d_null_blocked_out_returns_bl_err_null();

  return failures;
}
