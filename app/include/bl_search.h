#ifndef BL_SEARCH_H_
#define BL_SEARCH_H_

/**
 * @file bl_search.h
 * @brief 1D terrain search helpers for Blackline/SW.
 *
 * Provides bounded search and lookup routines for encoded terrain buffers.
 */

#include <stdbool.h>
#include <stddef.h>

#include "bl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Find the first agent cell in a 1D terrain buffer.
 *
 * On success, writes the zero-based cell index to @p idx_out.
 *
 * @param[in] cells Terrain cell buffer
 * @param[in] len Number of valid cells in the buffer
 * @param[out] idx_out Pointer to index storage
 * @return BL_OK on success, or a bl_status_t error code
 */
bl_status_t bl_find_agent_1d(const char *cells, size_t len, size_t *idx_out);

/**
 * @brief Find the first target cell in a 1D terrain buffer.
 *
 * On success, writes the zero-based cell index to @p idx_out.
 *
 * @param[in] cells Terrain cell buffer
 * @param[in] len Number of valid cells in the buffer
 * @param[out] idx_out Pointer to index storage
 * @return BL_OK on success, or a bl_status_t error code
 */
bl_status_t bl_find_target_1d(const char *cells, size_t len, size_t *idx_out);

/**
 * @brief Check whether a 1D terrain cell is blocked.
 *
 * On success, writes true to @p blocked_out when the requested cell contains
 * BL_CELL_BLOCKED; otherwise writes false.
 *
 * @param[in] cells Terrain cell buffer
 * @param[in] len Number of valid cells in the buffer
 * @param[in] idx Zero-based cell index to inspect
 * @param[out] blocked_out Pointer to blocked-state storage
 * @return BL_OK on success, or a bl_status_t error code
 */
bl_status_t bl_is_blocked_1d(const char *cells, size_t len, size_t idx,
                             bool *blocked_out);

#ifdef __cplusplus
}
#endif

#endif /* BL_SEARCH_H_ */
