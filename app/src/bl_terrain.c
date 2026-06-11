#include <stddef.h>

#include "bl_terrain.h"

static bl_status_t validate_input_1d(const char *cells, size_t len) {
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

  bl_status_t ret = validate_input_1d(cells, len);
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
