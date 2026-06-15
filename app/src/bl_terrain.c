#include <stddef.h>

#include "bl_terrain.h"

static bl_status_t validate_terrain_buffer_1d(const char *cells, size_t len) {
  if (cells == NULL) {
    return BL_ERR_NULL;
  }

  if (len == 0U) {
    return BL_ERR_INVALID_ARG;
  }

  if (len > BL_MAX_1D_CELLS) {
    return BL_ERR_BOUNDS;
  }

  return BL_OK;
}

static bl_status_t locate_marker_1d(const char *cells, size_t len, char marker,
                                    size_t *idx_out) {
  if (idx_out == NULL) {
    return BL_ERR_NULL;
  }

  bl_status_t ret = validate_terrain_buffer_1d(cells, len);
  if (ret != BL_OK) {
    return ret;
  }

  for (size_t i = 0U; i < len; ++i) {
    if (cells[i] == marker) {
      *idx_out = i;
      return BL_OK;
    }
  }

  return BL_ERR_NOT_FOUND;
}

static bl_status_t locate_block(const char *cells, size_t len, size_t idx,
                                bool *is_blocked) {
  if (is_blocked == NULL) {
    return BL_ERR_NULL;
  }

  bl_status_t ret = validate_terrain_buffer_1d(cells, len);
  if (ret != BL_OK) {
    return ret;
  }

  if (idx >= len) {
    return BL_ERR_BOUNDS;
  }

  if (cells[idx] == BL_CELL_BLOCKED) {
    *is_blocked = true;
  } else {
    *is_blocked = false;
  }

  return BL_OK;
}

bl_status_t bl_terrain_locate_agent_1d(const char *cells, size_t len,
                                       size_t *idx_out) {
  return locate_marker_1d(cells, len, BL_CELL_AGENT, idx_out);
}

bl_status_t bl_terrain_locate_target_1d(const char *cells, size_t len,
                                        size_t *idx_out) {
  return locate_marker_1d(cells, len, BL_CELL_TARGET, idx_out);
}

bl_status_t bl_terrain_is_blocked_1d(const char *cells, size_t len, size_t idx,
                                     bool *blocked_out) {
  return locate_block(cells, len, idx, blocked_out);
}
