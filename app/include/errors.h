/*****************************************************************************
 * @file errors.h
 *
 * @brief
 *   Defines application-specific error codes used for exception handling,
 *   particularly for signaling construction failures in classes that enforce
 *   strong invariants.
 *
 * @details
 *   This header introduces a minimal enumeration, `app::errors`, representing
 *   well-defined error conditions that may occur within the application.
 *   These values are intended to be used when throwing exceptions from
 *   constructors or other critical initialization paths where object creation
 *   must not proceed in an invalid state.
 *
 *   Having a dedicated error enumeration:
 *     * Ensures consistent and expressive exception signaling.
 *     * Avoids relying on arbitrary integers or magic constants.
 *     * Allows future expansion of error categories while maintaining clarity.
 *
 * @enum app::errors
 *   @var construction
 *        Indicates that object creation failed due to invalid parameters or
 *        an inability to satisfy required initialization conditions.
 *
 *****************************************************************************/
#ifndef _ERRORS_H_
#define _ERRORS_H_
namespace app {
        enum class errors {
                construction = 1
        };
}
#endif
