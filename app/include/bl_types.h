#ifndef BL_TYPES_H_
#define BL_TYPES_H_

/**
 * @file bl_types.h
 * @brief Shared constants and status codes for Blackline/SW.
 *
 * Defines the bounded 1D terrain vocabulary used by the portable search
 * backend.
 */

/** Maximum number of cells accepted by 1D terrain search functions. */
#define BL_MAX_1D_CELLS 64U

/** Encoded cell containing the agent position. */
#define BL_CELL_AGENT 'A'
/** Encoded cell containing the target position. */
#define BL_CELL_TARGET 'T'
/** Encoded cell that cannot be traversed. */
#define BL_CELL_BLOCKED '#'

#ifdef __cplusplus
extern "C" {
#endif

/** Status codes returned by Blackline/SW APIs. */
typedef enum {
  /** Operation completed successfully. */
  BL_OK = 0,
  /** Required pointer argument was NULL. */
  BL_ERR_NULL,
  /** Argument value was present but invalid for the operation. */
  BL_ERR_INVALID_ARG,
  /** Index, coordinate, or length exceeded a defined bound. */
  BL_ERR_BOUNDS,
  /** Requested marker or value was not present. */
  BL_ERR_NOT_FOUND
} bl_status_t;

#ifdef __cplusplus
}
#endif

#endif /* BL_TYPES_H_ */
