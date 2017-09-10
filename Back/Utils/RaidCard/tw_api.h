/**
 * $Id: tw_api.h,v 1.337 2010/08/10 01:19:11 sbukhari Exp $
 *
 * 3ware controller API
 *
 * http://www.3ware.com
 * 
 * 
 * Copyright (c) 2004, 2005 Applied Micro Circuits Corporation (AMCC).
 *   All rights reserved.
 * Copyright (c) 2002, 2003, 2004 3ware, Inc.  All rights reserved.
 *
 * This source code is AMCC confidential information and may not be
 * used without an express written license agreement from AMCC.
 * This source code may not be distributed or provided in any way to
 * a third party without the express written consent of AMCC.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef    _TW_API_H
#define    _TW_API_H

#include "tw_api_types.h"


#ifdef    __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 *Pre-Processor Definitions
 *
 */

#define TW_API_VERSION                            "2.08.00.021"



/*******************************************************************************
 * Error Information and Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the source and type of error regarding a failed request.
 *      This error information is significant only when an API routine has
 *      failed.  API routines that succeed may change this information.
 *      This routine will NOT update the error information.
 *
 *    Parameters:
 *      error_information   - The address where tw_get_error_information will
 *                              store the error information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Error information is available in the specified
 *                              location.
 *      TW_RET_FAILED       - The value of error_information is NULL.
 *                              Error information is NOT available in the
 *                              specified location.
 *
 *    Remarks:
 *      Applications that make calls to routines in this API should always
 *      check the return value.  When an API routine returns a value of
 *      "TW_RET_FAILED" the calling application can call this routine to
 *      get more information about the failed request.  Error information is
 *      stored in a struct of type tw_api_error_type.  The following fields
 *      within the error struct are of interest:
 *           valid_error_flags      - Indicates which error codes are valid.
 *                                    This is a bitwise OR'ed combination of
 *                                    the following:
 *                                      TW_ERROR_SOURCE_API
 *                                      TW_ERROR_SOURCE_OS
 *                                      TW_ERROR_SOURCE_DRIVER
 *                                      TW_ERROR_SOURCE_CONTROLLER
 *           api_error_code         - If valid, the API error code.  This will
 *                                    be a value in the range of:
 *                                      TW_ERROR_FIRST to 
 *                                      TW_ERROR_LAST
 *                                    See API error codes in tw_api_defines.h
 *                                    for specific values.
 *
 *           os_error_code          - If valid, the OS error code.  This will
 *                                    be the value corresponding to "errno" for
 *                                    UNIX and "GetLastError()" for windows.
 *                                    See OS documentation for information on
 *                                    specific values.
 *           driver_error_code      - If valid, the driver error code.
 *           controller_error_code  - If valid, the controller error code.
 *           text_length            - Length of the text field not including
 *                                    the terminating \0.
 *           text                   - Text description for each of the valid
 *                                    error codes.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_error_information ( tw_api_error_type*   error_information );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the source and type of error regarding a failed request.
 *      This error information is significant only when an API routine has
 *      failed.  API routines that succeed may change this information.
 *      This routine will NOT update the error information.  This is identical
 *      to tw_get_error_information, except it takes 2 more arguments
 *      described below
 *
 *    Parameters:
 *      error_information   - The address where tw_get_error_information2 will
 *                              store the error information.
 *
 *      language            - The language to which the error text should be
 *                              translated.
 *
 *      path                - The pathname to the language message catalogs
 *                              (without the trailing slash)
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Error information is available in the specified
 *                              location.
 *      TW_RET_FAILED       - The value of error_information is NULL.
 *                              Error information is NOT available in the
 *                              specified location.
 *
 *    Remarks:
 *      Applications that make calls to routines in this API should always
 *      check the return value.  When an API routine returns a value of
 *      "TW_RET_FAILED" the calling application can call this routine to
 *      get more information about the failed request.  Error information is
 *      stored in a struct of type tw_api_error_type.  The following fields
 *      within the error struct are of interest:
 *           valid_error_flags      - Indicates which error codes are valid.
 *                                    This is a bitwise OR'ed combination of
 *                                    the following:
 *                                      TW_ERROR_SOURCE_API
 *                                      TW_ERROR_SOURCE_OS
 *                                      TW_ERROR_SOURCE_DRIVER
 *                                      TW_ERROR_SOURCE_CONTROLLER
 *           api_error_code         - If valid, the API error code.  This will
 *                                    be a value in the range of:
 *                                      TW_ERROR_FIRST to 
 *                                      TW_ERROR_LAST
 *                                    See API error codes in tw_api_defines.h
 *                                    for specific values.
 *
 *           os_error_code          - If valid, the OS error code.  This will
 *                                    be the value corresponding to "errno" for
 *                                    unix and "GetLastError()" for windows.
 *                                    See OS documentation for information on
 *                                    specific values.
 *           driver_error_code      - If valid, the driver error code.
 *           controller_error_code  - If valid, the controller error code.
 *           text_length            - Length of the text field not including
 *                                    the terminating \0.
 *           text                   - Text description for each of the valid
 *                                    error codes.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_error_information2 (
      tw_api_error_type*   error_information,
      unsigned char        language,
      const char*          path
                              );


/*******************************************************************************
 * API Status and Monitoring Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the address of the string constant representing
 *      the version number of this API.
 *
 *    Parameters:
 *      None
 *
 *    Return Values:
 *      The address of the API version string constant, or
 *      TW_RET_FAILED if the request fails.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
const char*  tw_get_api_version ( void );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the address of the string constant representing
 *      the copyright of this API.
 *
 *    Parameters:
 *      none
 *
 *    Return Values:
 *      The address of the API copyright string constant, or
 *      TW_RET_FAILED if the request fails.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
const char*  tw_get_api_copyright ( void );


/*******************************************************************************
 * Controller Status and Monitoring Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides a list of the 3ware controllers in the system.  The API
 *      probes the system to determine the number and IDs of 3ware
 *      controllers, if present.  The IDs are written to the location
 *      specified by controller_id_list.  The number of 3ware controllers in
 *      the system is written to the length element of the controller id list.
 *
 *    Parameters:
 *      controller_id_list  - The address where
 *                              tw_get_controller_id_list will store
 *                              the controller id list and length.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The controller id list and length are
 *                              available in the specified location.
 *      TW_RET_FAILED       - The controller id list is NOT valid and
 *                              the list length is set to zero.
 *
 *    Remarks:
 *      Applications that make calls to other routines in this API to get
 *      or set controller related information must provide the ID of the
 *      controller of interest.  This routine will provide a list of all
 *      3ware controllers, by ID, within an system.  Controller ID list is
 *      stored in a struct of type tw_controller_id_list_type.
 *      The following fields within the list struct are of interest:
 *           list                   - An array of <length> controller IDs.
 *           length                 - The number of controller IDs in the list.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_controller_id_list( tw_controller_id_list_type* controller_id_list );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the device driver version of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      version of the specified 3ware controller's device driver.  The version
 *      of the specified controller's device driver is written to the location
 *      specified by the driver_version parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      driver_version      - The address where tw_get_driver_version will
 *                              store the controller's device driver version.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The version of the specified controller's device
 *                            driver is available in the specified location.
 *      TW_RET_FAILED     - The version of the specified controller's device
 *                            driver is NOT available in the specified location.
 *
 *    Remarks:
 *      Controllers in a system have a corresponding device driver to provide
 *      required services to the operating system.  This routine provides the
 *      version of a controller's device driver.  It is possible for different
 *      controller series (7000, 8000, and 9000) to have different device
 *      drivers.  This routine can be used to identify the device driver that
 *      is used with a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_driver_version (
      tw_controller_id_type     controller_id,
      tw_driver_version_type*   driver_version
                          );

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the architecture ID of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests
 *      the architecture ID from the specified controller.  The architecture
 *      ID is written to the location specified by architecture_id.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      architecture_id     - The address where tw_get_architecture_id will
 *                              store the controller architecture id.
 *                              The architecture ID value is one of:
 *                                TW_API_UNKNOWN_ARCHITECTURE
 *                                TW_API_7000_ARCHITECTURE
 *                                TW_API_8000_ARCHITECTURE
 *                                TW_API_9000_ARCHITECTURE
 *                                TW_API_9550_ARCHITECTURE
 *                                TW_API_9690_ARCHITECTURE
 *                                TW_API_9750_ARCHITECTURE
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The architecture id for the specified
 *                              controller is available in the
 *                              specified location.
 *      TW_RET_FAILED       - The architecture id for the specified
 *                              controller is NOT available in the
 *                              specified location.
 *
 *    Remarks:
 *      3ware Controllers have an architecture ID that provides a way to
 *      distinguish features and capabilities of a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_architecture_id (
      tw_controller_id_type      controller_id,
      tw_architecture_id_type*   architecture_id
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the model of the specified 3ware controller.  The API
 *      verifies that the controller ID is valid, then requests the model
 *      from the specified controller.  The model of the specified controller
 *      is written to the location specified by the model parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      model               - The address where tw_get_model will store
 *                              the controller model.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The model for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The model for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have an associated model that identifies the
 *      hardware family to which the controller belongs.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_model (
      tw_controller_id_type   controller_id,
      tw_model_type*          model
                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the serial number of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      serial number from the specified controller.  The serial number of
 *      the specified controller is written to the location specified by the
 *      serial_number parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      serial_number       - The address where tw_get_serial_number will
 *                              store the controller serial number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The serial number for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The serial number for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have an associated serial number that uniquely
 *      identifies the controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_serial_number (
      tw_controller_id_type    controller_id,
      tw_serial_number_type*   serial_number
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the PCB revision of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      PCB revision from the specified controller.  The PCB revision of
 *      the specified controller is written to the location specified by the
 *      pcb_revision parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      pcb_revision        - The address where tw_get_pcb_revision will
 *                              store the controller PCB revision.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The PCB revision for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The PCB revision for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have an associated pcb revision that identifies
 *      the hardware revision of the controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_pcb_revision (
      tw_controller_id_type   controller_id,
      tw_pcb_revision_type*   pcb_revision
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the A-chip version of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      A-chip version from the specified controller.  The A-chip version of
 *      the specified controller is written to the location specified by the
 *      a_chip_version parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      a_chip_version      - The address where tw_get_a_chip_version will
 *                              store the controller A-chip version.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The A-chip version for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The A-chip version for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have an associated A-chip (ATA interface chip).
 *      This routine provides the version of that A-chip.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_a_chip_version (
      tw_controller_id_type     controller_id,
      tw_a_chip_version_type*   a_chip_version
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the P-chip version of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      P-chip version from the specified controller.  The P-chip version of
 *      the specified controller is written to the location specified by the
 *      p_chip_version parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      p_chip_version      - The address where tw_get_p_chip_version will
 *                              store the controller P-chip version.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The P-chip version for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The P-chip version for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have an associated P-chip (PCI interface chip).
 *      This routine provides the version of that P-chip.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_p_chip_version (
      tw_controller_id_type     controller_id,
      tw_p_chip_version_type*   p_chip_version
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the firmware version of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      firmware version from the specified controller.  The firmware version
 *      of the specified controller is written to the location specified by
 *      the firmware_version parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      firmware_version    - The address where tw_get_firmware_version will
 *                              store the controller firmware version.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The firmware version for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The firmware version for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have associated controller firmware.  This routine
 *      provides the version of the firmware on a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_firmware_version (
      tw_controller_id_type       controller_id,
      tw_firmware_version_type*   firmware_version
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the BIOS version of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      BIOS version from the specified controller.  The BIOS version of
 *      the specified controller is written to the location specified by the
 *      bios_version parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bios_version        - The address where tw_get_bios_version will
 *                              store the controller BIOS version.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The BIOS version for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The BIOS version for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have associated option ROM bios.  This routine
 *      provides the version of the option ROM bios on a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bios_version (
      tw_controller_id_type   controller_id,
      tw_bios_version_type*   bios_version
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *     Description:
 *       Provides the monitor version of the specified 3ware controller.
 *       The API verifies that the controller ID is valid, then requests the
 *       monitor version from the specified controller.  The monitor version
 *       of the specified controller is written to the location specified by
 *       the monitor_version parameter.  For 9000 series controllers this is
 *       also known as the "boot loader" version.
 *
 *     Parameters:
 *       controller_id      - ID of the controller in question.
 *       monitor_version    - The address where tw_get_monitor_version will
 *                              store the controller monitor version.
 *
 *     Return Values:
 *       TW_RET_SUCCESS     - The monitor version for the specified controller
 *                              is available in the specified location.
 *       TW_RET_FAILED      - The monitor version for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have associated firmware monitor.  This routine
 *      provides the version of the firmware monitor on a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_monitor_version (
      tw_controller_id_type      controller_id,
      tw_monitor_version_type*   monitor_version
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of ports on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of ports from the specified controller.  The number of ports
 *      on the specified controller is written to the location specified by
 *      the number_of_ports parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      number_of_ports     - The address where tw_get_number_of_ports will
 *                              store the number of ports on the specified
 *                              controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of ports for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The number of ports for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have a fixed number of disk drive ports on each
 *      controller.  This routine provides the number of disk drive ports
 *      on a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_ports (
      tw_controller_id_type   controller_id,
      int*                    number_of_ports
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of drives on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of drives from the specified controller.  The number of drives
 *      on the specified controller is written to the location specified by
 *      the number_of_drives parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      number_of_drives    - The address where tw_get_number_of_drives will
 *                              store the number of drives attached to the
 *                              specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of drives for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The number of drives for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      This routine provides the number of disk drives attached to a
 *      particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_ports 
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_drives (
      tw_controller_id_type   controller_id,
      int*                    number_of_drives
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of units on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of units from the specified controller.  The number of units
 *      on the specified controller is written to the location specified by
 *      the number_of_units parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      number_of_units     - The address where tw_get_number_of_units will
 *                              store the number of units associated with
 *                              the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of units for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The number of units for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      This routine provides the number of storage units associated with a
 *      particular controller.  Storage units, or "units" are the lowest
 *      level mass storage object visible to the operating system (OS).
 *      A five drive RAID 0 unit is looks like a large "disk drive" to the OS.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_units (
      tw_controller_id_type   controller_id,
      int*                    number_of_units
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the background rebuild rate of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      background rebuild rate from the specified controller.  The background
 *      rebuild rate of the specified controller is written to the location
 *      specified by the background_rebuild_rate parameter.
 *
 *    Parameters:
 *      controller_id             - ID of the controller in question.
 *      background_rebuild_rate   - The address where
 *                                    tw_get_background_rebuild_rate will store
 *                                    the controller background rebuild rate.
 *                                    The rebuild rate value is one of:
 *                                      TW_BACKGROUND_TASK_RATE_LOW
 *                                      TW_BACKGROUND_TASK_RATE_MED_LOW
 *                                      TW_BACKGROUND_TASK_RATE_MEDIUM
 *                                      TW_BACKGROUND_TASK_RATE_MED_HI
 *                                      TW_BACKGROUND_TASK_RATE_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS            - The background rebuild rate for the
 *                                    specified controller is available
 *                                    in the specified location.
 *      TW_RET_FAILED             - The background rebuild rate for the
 *                                    specified controller is NOT available
 *                                    in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the rate at which a particular controller
 *      rebuilds redundant units (RAID 1, RAID 5, RAID 10, RAID 50).
 *      The task rate has the following effects on performance during a
 *      rebuild operation:
 *        TW_BACKGROUND_TASK_RATE_LOW       Lowest   task rate, highest IO rate
 *        TW_BACKGROUND_TASK_RATE_MED_LOW   Lower    task rate, higher  IO rate
 *        TW_BACKGROUND_TASK_RATE_MEDIUM    Balanced task and IO rates
 *        TW_BACKGROUND_TASK_RATE_MED_HI    Higher   task rate, lower   IO rate
 *        TW_BACKGROUND_TASK_RATE_HIGH      Highest  task rate, lowest  IO rate
 *
 *    See also:
 *      tw_set_background_rebuild_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_background_rebuild_rate (
      tw_controller_id_type           controller_id,
      tw_background_task_rate_type*   background_rebuild_rate
                                   );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Provides the Background Task Activity Policy(BTAP) setting for rebuild task
 *      on the specified 3ware controller. The API verifies that the controller ID is valid,
 *      then requests the BTAP setting from the specified controller.
 *      The value is written to the location specified by the btap_policy parameter.
 *
 *    Parameters:
 *      controller_id             - ID of the controller in question.
 *      btap_policy               - The address where
 *                                    tw_get_background_rebuild_activity_policy will store
 *                                    the BTAP value and is one of:
 *                                      TW_BTAP_POLICY_ADAPTIVE
 *                                      TW_BTAP_POLICY_LOW_LATENCY
 *
 *    Return Values:
 *      TW_RET_SUCCESS            - The background rebuild activity policy for the
 *                                    specified controller is available
 *                                    in the specified location.
 *      TW_RET_FAILED             - The background rebuild activity policy for the
 *                                    specified controller is NOT available
 *                                    in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the background task activity policy(BTAP) setting for rebuild task
 *      on a particular controller. Use BTAP setting along with the background rebuild rate for customers
 *      that have requirement for lower latency of host read commands. 
 *
 *        When background tasks are active (Rebuild, Verify, Init etc) increases latency significantly,
 *      if the task rate is set to lower IO rate. Using this feature, setting it to low latency mode
 *      will throttle the background task and allow the host read commands to complete within 100ms or less.
 *
 *    See also:
 *      tw_set_background_rebuild_activity_policy
 *      tw_get_background_rebuild_rate
 *      tw_set_background_rebuild_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_background_rebuild_activity_policy (
      tw_controller_id_type    controller_id,
      tw_btap_policy_type*     btap_policy
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the background verify rate of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      background verify rate from the specified controller.  The background
 *      verify rate of the specified controller is written to the location
 *      specified by the background_verify_rate parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      background_verify_rate   - The address where
 *                                   tw_get_background_verify_rate will store
 *                                   the controller background verify rate.
 *                                   The rebuild rate value is one of:
 *                                     TW_BACKGROUND_TASK_RATE_LOW
 *                                     TW_BACKGROUND_TASK_RATE_MED_LOW
 *                                     TW_BACKGROUND_TASK_RATE_MEDIUM
 *                                     TW_BACKGROUND_TASK_RATE_MED_HI
 *                                     TW_BACKGROUND_TASK_RATE_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The background verify rate for the
 *                              specified controller is available
 *                              in the specified location.
 *      TW_RET_FAILED       - The background verify rate for the
 *                              specified controller is NOT available
 *                              in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the rate at which a particular controller
 *      verifies units.  The task rate has the following effects on performance
 *      during a verify operation:
 *        TW_BACKGROUND_TASK_RATE_LOW       Lowest   task rate, highest IO rate
 *        TW_BACKGROUND_TASK_RATE_MED_LOW   Lower    task rate, higher  IO rate
 *        TW_BACKGROUND_TASK_RATE_MEDIUM    Balanced task and IO rates
 *        TW_BACKGROUND_TASK_RATE_MED_HI    Higher   task rate, lower   IO rate
 *        TW_BACKGROUND_TASK_RATE_HIGH      Highest  task rate, lowest  IO rate
 *
 *    See also:
 *      tw_set_background_verify_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_background_verify_rate (
      tw_controller_id_type           controller_id,
      tw_background_task_rate_type*   background_verify_rate
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Provides the Background Task Activity Policy(BTAP) setting for verify task
 *      on the specified 3ware controller. The API verifies that the controller ID is valid,
 *      then requests the BTAP setting from the specified controller.
 *      The value is written to the location specified by the btap_policy parameter.
 *
 *    Parameters:
 *      controller_id             - ID of the controller in question.
 *      btap_policy               - The address where
 *                                    tw_get_background_verify_activity_policy will store
 *                                    the BTAP value and is one of:
 *                                      TW_BTAP_POLICY_ADAPTIVE
 *                                      TW_BTAP_POLICY_LOW_LATENCY
 *
 *    Return Values:
 *      TW_RET_SUCCESS            - The background verify activity policy for the
 *                                    specified controller is available
 *                                    in the specified location.
 *      TW_RET_FAILED             - The background verify activity policy for the
 *                                    specified controller is NOT available
 *                                    in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the background task activity policy(BTAP) setting for verify task
 *      on a particular controller. Use BTAP setting along with the background verify rate for customers
 *      that have requirement for lower latency of host read commands. 
 *
 *        When background tasks are active (Rebuild, Verify, Init etc) increases latency significantly,
 *      if the task rate is set to lower IO rate. Using this feature, setting it to low latency mode
 *      will throttle the background task and allow the host read commands to complete within 100ms or less.
 *
 *    See also:
 *      tw_set_background_verify_activity_policy
 *      tw_get_background_verify_rate
 *      tw_set_background_verify_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_background_verify_activity_policy (
      tw_controller_id_type    controller_id,
      tw_btap_policy_type*     btap_policy
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the test enable flags of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      test enable flags from the specified controller.  The test enable
 *      flags are written to the location specified by the test_enable_flags
 *      parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      test_enable_flags   - The address where tw_get_selftest_flags will
 *                              store this controller's test enable flags.
 *                              This is a bitwise OR'ed combination of the
 *                              following selftest flags:
 *                                TW_SELFTEST_ENABLE_UPGRADE_DMA_MODE
 *                                TW_SELFTEST_ENABLE_CHECK_SMART_THRESHOLD
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The selftest flags for the specified controller
 *                              are available in the specified location.
 *      TW_RET_FAILED       - The selftest flags for the specified controller
 *                              are NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read which selftests are enabled on a particular
 *      controller.
 *
 *    See also:
 *      tw_set_selftest_flags
 *      tw_get_selftest_task_item
 *      tw_set_selftest_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_selftest_flags (
      tw_controller_id_type       controller_id,
      tw_selftest_enable_flags*   test_enable_flags
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the bit mask of supported selftest from the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      supported selftest from the specified controller.  The value is
 *      written to the location specified by the test_supported parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      test_supported      - The address where tw_get_selftest_capability will
 *                              store this controller's supported selftest.
 *                              This is a bitwise OR'ed combination of the
 *                              following selftest flags:
 *                                TW_SELFTEST_SUPPORT_UPGRADE_DMA_MODE
 *                                TW_SELFTEST_SUPPORT_CHECK_SMART_THRESHOLD
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The supported selftest flags for the specified controller
 *                              are available in the specified location.
 *      TW_RET_FAILED       - The supported selftest flags for the specified controller
 *                              are NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read which selftests are supported on a particular
 *      controller.
 *
 *    See also:
 *      tw_set_selftest_flags
 *      tw_get_selftest_task_item
 *      tw_set_selftest_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_selftest_capability (
      tw_controller_id_type    controller_id,
      tw_selftest_supported*   test_supported
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides rebuild schedule's state of the specified controller.
 *    The API verifies that the controller ID is valid, then requests the
 *    rebuild schedule state from the specified controller.
 *    The rebuild schedule state of the controller is written to the
 *    location specified by the rebuild_schedule_state parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      rebuild_schedule_state   - The address where
 *                                  tw_get_rebuild_schedule_state will store
 *                                  the rebuild schedule state.
 *                                    Rebuild schedule state value is one of:
 *                                      TW_TASK_CONTROL_DISABLED
 *                                      TW_TASK_CONTROL_SCHEDULE
 *                                      TW_TASK_CONTROL_DRIVER_LOAD
 *
 *    Return Values:
 *      TW_RET_SUCCESS           - The rebuild schedule state for the
 *                                   specified controller is available
 *                                   in the specified location.
 *      TW_RET_FAILED            - The rebuild schedule state for the
 *                                   specified controller is NOT available
 *                                   in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the state of a particular controller's
 *      rebuild schedule.  The rebuild schedule state can have one of the
 *      following values:
 *        TW_TASK_CONTROL_DISABLED    - Rebuild operations are performed only
 *                                        through rebuild_unit commands.
 *                                        The rebuild schedule is ignored.
 *        TW_TASK_CONTROL_SCHEDULE    - Rebuild operations are performed 
 *                                        according to the rebuild schedule.
 *        TW_TASK_CONTROL_DRIVER_LOAD - Rebuild operations are performed
 *                                        immediately after driver is loaded
 *                                        for any units in rebuild state.
 *                                        The schedule table is ignored.
 *
 *    See also:
 *      tw_enable_rebuild_task_schedule
 *      tw_disable_rebuild_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_rebuild_schedule_state (
      tw_controller_id_type     controller_id,
      tw_schedule_state_type*   rebuild_schedule_state
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides verify schedule's state of the specified controller.
 *    The API verifies that the controller ID is valid, then requests the
 *    verify schedule state from the specified controller.
 *    The verify schedule state of the controller is written to the
 *    location specified by the verify_schedule_state parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      verify_schedule_state    - The address where
 *                                   tw_get_verify_schedule_state will
 *                                   store the verify schedule state.
 *                                    Verify schedule state value is one of:
 *                                      TW_TASK_CONTROL_DISABLED
 *                                      TW_TASK_CONTROL_SCHEDULE
 *                                      TW_TASK_CONTROL_DRIVER_LOAD
 *
 *    Return Values:
 *      TW_RET_SUCCESS           - The verify schedule state for the
 *                                   specified controller is available
 *                                   in the specified location.
 *      TW_RET_FAILED            - The verify schedule state for the
 *                                   specified controller is NOT available
 *                                   in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the state of a particular controller's
 *      verify schedule.  The verify schedule state can have one of the
 *      following values:
 *        TW_TASK_CONTROL_DISABLED    - Verify operations are performed only
 *                                        through verify_unit commands.
 *                                        The verify schedule is ignored.
 *        TW_TASK_CONTROL_SCHEDULE    - Verify operations are performed 
 *                                        according to the verify schedule.
 *        TW_TASK_CONTROL_DRIVER_LOAD - Verify operations are performed
 *                                        immediately after driver is loaded
 *                                        for any units in verify state.
 *                                        The schedule table is ignored.
 *
 *    See also:
 *      tw_disable_verify_task_schedule
 *      tw_enable_verify_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_verify_schedule_state (
      tw_controller_id_type     controller_id,
      tw_schedule_state_type*   verify_schedule_state
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Provides the start time and duration of the specified rebuild task
 *      on the specified 3ware controller.  The API verifies that the
 *      controller ID and task number are valid, then requests the start time
 *      and duration from the specified controller.  The start time and duration
 *      are written to the locations specified by the start_time and duration
 *      parameters respectively.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The rebuild task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The address where tw_get_rebuild_task_item will
 *                              store this rebuild task's start time.
 *                              Value will be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *      duration            - The address where tw_get_rebuild_task_item will
 *                              store this rebuild task's duration.
 *                              Value will be in the range of:
 *                                TW_MIN_TEST_DURATION ..
 *                                TW_MAX_TEST_DURATION
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified rebuild task
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The schedule for the specified rebuild task
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular rebuild task schedule from a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_set_rebuild_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_rebuild_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type*      start_time,
      tw_task_item_type*      duration
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Provides the start time and duration of the specified verify task
 *      on the specified 3ware controller.  The API verifies that the
 *      controller ID and task number are valid, then requests the start time
 *      and duration from the specified controller.  The start time and duration
 *      are written to the locations specified by the start_time and duration
 *      parameters respectively.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The verify task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The address where tw_get_verify_task_item will
 *                              store this verify task's start time.
 *                              Value will be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *      duration            - The address where tw_get_verify_task_item will
 *                              store this verify task's duration.
 *                              Value will be in the range of:
 *                                TW_MIN_TEST_DURATION ..
 *                                TW_MAX_TEST_DURATION
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified verify task
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The schedule for the specified verify task
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular verify task schedule from a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_set_verify_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_verify_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type*      start_time,
      tw_task_item_type*      duration
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Use this routine to determine whether the basic verify feature
 *      supported on on the specified 3ware controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      yes_no              - The address where tw_is_basic_verify_supported will
 *                              store the value.
 *                              will be one of:
 *                                TW_TRUE
 *                                TW_FALSE
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Succeeded and the value
 *                              is available in the specified location.
 *      TW_RET_FAILED       - Failed. see tw_get_api_error_info() for more details.
 *
 *    Remarks:
 *     
 *
 *    See also:
 *      tw_get_verify_preferred_time
 *      tw_set_verify_preferred_time
 *      tw_disable_verify_task_schedule
 *      tw_enable_verify_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_is_basic_verify_supported (
      tw_controller_id_type    controller_id,
      int*                     yes_no
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Provides the preferred start time of the basic verify task
 *      on the specified 3ware controller.  The API verifies that the
 *      controller ID is valid, then requests the preferred start time
 *      from the specified controller.  The preferred start time is
 *      written to the locations specified by the start_time parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      start_time          - The address where tw_get_verify_preferred_time will
 *                              store the preferred verify start time.
 *                              Value will be in the range of:
 *                                TW_MIN_PREFERRED_START_TIME ..
 *                                TW_MAX_PREFERRED_START_TIME
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The preferred verify start time
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The preferred verify start time
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a preferred verify start time from a
 *      particular controller. The preferred start time is in minutes and
 *      only represents the time within a given week.
 *      For example:   0 = SUNDAY midnight,  1500 = MONDAY 1:00am
 *      
 *      NOTE:
 *        The controller runs verify task at preferred verify start time
 *      once a week, if the controller's verify task schedule setting is
 *      disabled through the tw_disable_verify_task_schedule API.
 *
 *    See also:
 *      tw_set_verify_preferred_time
 *      tw_disable_verify_task_schedule
 *      tw_enable_verify_task_schedule
 *      tw_get_verify_task_item
 *      tw_set_verify_task_item
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_verify_preferred_time (
      tw_controller_id_type    controller_id,
      tw_preferred_time_type*  start_time
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Provides the start time and test enable flags of the specified
 *      selftest task on the specified 3ware controller.  The API verifies
 *      that the controller ID and the task number are valid, then requests
 *      the start time and test enable flags from the specified controller.
 *      The start time and test enable flags are written to the locations
 *      specified by the start_time and test_enable_flags parameters
 *      respectively.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The selftest task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The address where tw_get_selftest_task_item will
 *                              store this selftest task's start time.
 *                              Value will be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified selftest task
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The schedule for the specified selftest task
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular selftest task schedule from a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_set_selftest_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_selftest_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type*      start_time
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the task schedule of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      task schedule from the specified controller.  The task schedule
 *      of the specified controller is written to the location specified by
 *      the task_schedule parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_schedule       - The address where tw_get_task_schedule will
 *                              store the controller task schedule.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The task schedule for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The task schedule for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read an entire task schedule from a particular 
 *      controller.  This includes rebuild, verify and selftest tasks.
 *
 *    See also:
 *      tw_set_task_schedule
 *      tw_get_rebuild_task_item
 *      tw_get_verify_task_item
 *      tw_get_selftest_task_item
 *      tw_set_rebuild_task_item
 *      tw_set_verify_task_item
 *      tw_set_selftest_task_item
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_task_schedule (
      tw_controller_id_type    controller_id,
      tw_task_schedule_type*   task_schedule
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Provides the number of spinups allowed by the specified 3ware
 *      controller.  The API verifies that the controller ID is valid, then
 *      requests the number of spinups allowed from the specified controller.
 *      The number of spinups allowed by the specified controller is written
 *      to the location specified by the spinups_allowed parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      spinups_allowed     - The address where tw_get_spinups_allowed will
 *                              store the number of spinups allowed.
 *                              Value will be in the range of:
 *                                TW_MIN_SPINUPS_ALLOWED..
 *                                TW_MAX_SPINUPS_ALLOWED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of spinups allowed by the specified
 *                              controller is available in the
 *                              specified location.
 *      TW_RET_FAILED       - The number of spinups allowed by the specified
 *                              controller is NOT available in the
 *                              specified location.
 *
 *    Remarks:
 *      Use this routine to read the number of drives that will spinup,
 *      at one time, on a particular controller.  Systems may need to
 *      spinup a few drives at a time in order to reduce demands on the
 *      system power supply.
 *
 *    See also:
 *      tw_get_stagger_time
 *      tw_set_spinups_allowed
 *      tw_set_stagger_time
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_spinups_allowed (
      tw_controller_id_type      controller_id,
      tw_spinups_allowed_type*   spinups_allowed
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the stagger time of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      stagger time from the specified controller.  The stagger time of
 *      the specified controller is written to the location specified by the
 *      stagger_time parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      stagger_time        - The address where tw_get_stagger_time will
 *                              store the controller stagger time.
 *                              Value will be in the range of:
 *                                TW_MIN_STAGGER_TIME..
 *                                TW_MAX_STAGGER_TIME
 *                              Units are in seconds.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The stagger time for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The stagger time for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the delay between drive groups that will
 *      spinup, at one time, on a particular controller.  Systems may need
 *      to spinup a few drives at a time in order to reduce demands on the
 *      system power supply.
 *
 *    See also:
 *      tw_get_spinups_allowed
 *      tw_set_spinups_allowed
 *      tw_set_stagger_time
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_stagger_time (
      tw_controller_id_type   controller_id,
      tw_stagger_time_type*   stagger_time
                        );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the rollcall pause time of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      rollcall pause from the specified controller.  The rollcall pause of
 *      the specified controller is written to the location specified by the
 *      rollcall_pause parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      rollcall_pause      - The address where tw_get_rollcall_pause will
 *                              store the controller rollcall pause.
 *                              Value will be in the range of:
 *                                TW_MIN_ROLLCALL_PAUSE_TIME..
 *                                TW_MAX_ROLLCALL_PAUSE_TIME
 *                              Units are in seconds.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The rollcall pause for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The rollcall pause for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the controller setting for the initial pause
 *      or delay(in seconds) before drive rollcall/spinup begins on a particular controller.
 *      Systems may need to adjust this value in order to balance the demands
 *      on the system power supply.
 *
 *    See also:
 *      tw_set_rollcall_pause
 *      tw_get_spinups_allowed
 *      tw_set_spinups_allowed
 *      tw_get_stagger_time
 *      tw_set_stagger_time
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_rollcall_pause (
      tw_controller_id_type    controller_id,
      tw_rollcall_pause_type*  rollcall_pause
                        );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the JBOD policy of the specified controller.  The API
 *      verifies the controller ID, then requests the JBOD policy from
 *      the specified controller.  The JBOD policy of the specified
 *      controller is written to the location specified by the
 *      jbod_policy parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      jbod_policy         - The address where tw_get_jbod_policy will
 *                              store the jbod policy.
 *                              Value will be one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The JBOD policy for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The JBOD policy for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the JBOD policy on a particular controller.
 *      A value of TW_POLICY_DISABLED indicates that JBOD units are not
 *      available to the operating system.  A value of TW_POLICY_ENABLED
 *      indicates that JBOD units are available to the operating system.
 *
 *    See also:
 *      tw_disable_jbod_online
 *      tw_enable_jbod_online
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_jbod_policy (
      tw_controller_id_type   controller_id,
      tw_policy_state_type*   jbod_policy
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9500S only)
 *    Description:
 *      Retrieves the disable_cache_on_degrade policy of the specified 
 *      controller.  The API verifies the controller ID, then requests the 
 *      cache policy from the specified controller.  The disable_cache_on_degrade
 *      policy of the specified controller is written to the location specified 
 *      by the cache_policy parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      cache_policy        - The address where tw_get_cache_policy will
 *                              store the cache policy.
 *                              Value will be one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The cache policy for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The cache policy for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the disable_cache_on_degrade policy of a 
 *      particular controller. A value of TW_POLICY_DISABLED indicates that
 *      the controller cache will NOT be disabled when one of the controller's units degrade.
 *      A value of TW_POLICY_ENABLED indicates that the controller cache will
 *      be disabled when one of the controller's units degrade.
 *
 *    See also:
 *      tw_disable_cache_on_unit_degrade
 *      tw_use_unit_cache_states
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_cache_policy (
      tw_controller_id_type   controller_id,
      tw_policy_state_type*   cache_policy
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the ID of the boot unit of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      ID of the boot unit from the specified controller.  The ID of the
 *      boot unit of the specified controller is written to the location
 *      specified by the boot_unit parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      boot_unit_id        - The address where tw_get_boot_unit will store
 *                              the ID of the controller's boot unit.
 *                              Value will be
 *                                TW_NULL_UNIT
 *                              or in the range of:
 *                                TW_FIRST_UNIT..
 *                                TW_MAX_UNITS
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The boot unit ID for the specified controller is
 *                              available in the specified location.
 *      TW_RET_FAILED       - The boot unit ID for the specified controller is
 *                              NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read which unit a particular controller will
 *      present as a system boot target.  A value of TW_NULL_UNIT indicates
 *      that no unit is available as a system boot target.
 *
 *    See also:
 *      tw_set_boot_unit
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_boot_unit (
      tw_controller_id_type   controller_id,
      tw_unit_id_type*        boot_unit_id
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550SX and higher)
 *                                          Nvram controller
 *
 *    Description:
 *      This method returns PCI Mode of the controller.
 *
 *    Parameters:
 *      controller_id        - Controller ID
 *      mode                 - The address where tw_get_controller_bus_mode
 *                               will store the PCI mode value with
 *                               a bitwise OR'ed combination of the following:
 *                                 TW_HBA_BUS_PCI_MODE64
 *                                 TW_HBA_BUS_PCI_33
 *                                 TW_HBA_BUS_PCI_66
 *                                 TW_HBA_BUS_PCIX_66
 *                                 TW_HBA_BUS_PCIX_100
 *                                 TW_HBA_BUS_PCIX_133
 *                                 TW_HBA_BUS_PCIE_G1
 *                                 TW_HBA_BUS_PCIE_G2
 *                                 TW_HBA_BUS_PCIE_LANE_1X
 *                                 TW_HBA_BUS_PCIE_LANE_4X
 *                                 TW_HBA_BUS_PCIE_LANE_8X
 *                                 TW_HBA_BUS_PCIE_LANE_16X
 *                            
 *
 *    Remarks:
 *      Use this routine to read the PCI bus mode of a particular controller.
 *
 *      A value of TW_HBA_BUS_PCI_MODE64 indicates that the controller bus is 
 *      64-bit wide, if not set then it is 32-bit wide for both PCI & PCIX.
 *      PCIexpress ignore this bit.
 *
 *      A value of TW_HBA_BUS_PCI_33 indicates that the controller bus is PCI with 33 MHz bus speed.
 *
 *      A value of TW_HBA_BUS_PCI_66 indicates that the controller bus is PCI with 66 MHz bus speed.
 *
 *      A value of TW_HBA_BUS_PCIX_66 indicates that the controller bus is PCIX with 66 MHz bus speed.
 *
 *      A value of TW_HBA_BUS_PCIX_100 indicates that the controller bus is PCIX with 100 MHz bus speed.
 *
 *      A value of TW_HBA_BUS_PCIX_133 indicates that the controller bus is PCI with 133 MHz bus speed.
 *
 *      A value of TW_HBA_BUS_PCIE_G1 indicates that the controller bus is PCIexpress G1 bus speed.
 *
 *      A value of TW_HBA_BUS_PCIE_G2 indicates that the controller bus is PCIexpress G2 bus speed.
 *
 *      A value of TW_HBA_BUS_PCIE_LANE_1X indicates that the controller has 1 Lane PCIe.
 *
 *      A value of TW_HBA_BUS_PCIE_LANE_4X indicates that the controller has 4 Lane PCIe.
 *
 *      A value of TW_HBA_BUS_PCIE_LANE_8X indicates that the controller has 8 Lane PCIe.
 *
 *      A value of TW_HBA_BUS_PCIE_LANE_16X indicates that the controller has 16 Lane PCIe.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The bus mode for the specified controller
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The bus mode for the specified controller
 *                           is NOT available in the specified location.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_controller_bus_mode (
      tw_controller_id_type   controller_id,
      tw_hba_bus_type*        mode
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides AV mode state of the specified controller.
 *    The API verifies that the controller ID is valid, then requests the
 *    AV mode state from the specified controller.  The AV mode state of
 *    the controller is written to the location specified by the
 *    avmode_state parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      avmode_state             - The address where tw_get_avmode will store
 *                                  the AV mode state.
 *                                    AV mode state value is one of:
 *                                      TW_POLICY_DISABLED
 *                                      TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS           - The AV mode state for the
 *                                   specified controller is available
 *                                   in the specified location.
 *      TW_RET_FAILED            - The AV mode state for the
 *                                   specified controller is NOT available
 *                                   in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the state of a particular controller's
 *      AV mode.  The AV mode state can have one of the following values:
 *        TW_POLICY_DISABLED     - I/O operations are given normal retries
 *                                   ,timeout periods and normal error
 *                                   recovery. Self tests are
 *                                   performed according to the schedule.
 *                                   diagnostic data and errors are logged.
 *        TW_POLICY_ENABLED      - I/O operations are given fewer retries
 *                                   shortened error recovery cycle.
 *                                   All self tests and diagnostic data logging are disabled.
 *
 *    See also:
 *      tw_enable_avmode
 *      tw_disable_avmode
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_avmode (
      tw_controller_id_type     controller_id,
      tw_avmode_state_type*     avmode_state
                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *
 *    Description:
 *      Provides the diagnostic log of the specified controller.  The API
 *      verifies that the controller ID is valid, then requests the diagnostic
 *      log of the specified controller.  The diagnostic log of the controller
 *      is written to the location specified by the diagnostic_log parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      diagnostic_log      - The address where tw_get_diagnostic_log
 *                              will store the controller diagnostic log.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The diagnostic log for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The diagnostic log for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular controller's diagnostic log.
 *      This log may be requested by 3ware customer support in order to
 *      resolve operational issues.  The diagnostic log is, typically,
 *      not as recent as the controller log.
 *
 *    See also:
 *      tw_get_controller_log
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_diagnostic_log (
      tw_controller_id_type     controller_id,
      tw_diagnostic_log_type*   diagnostic_log
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the controller log of the specified controller.  The API
 *      verifies that the controller ID is valid, then requests the controller
 *      log of the specified controller.  The controller log of the controller
 *      is written to the location specified by the controller_log parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      controller_log      - The address where tw_get_controller_log
 *                              will store the controller log.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The controller log for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The controller log for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular controller's controller log.
 *      This log may be requested by 3ware customer support in order to
 *      resolve operational issues.  The controller log is, typically, more
 *      recent than the diagnostic log.
 *
 *    See also:
 *      tw_get_diagnostic_log
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_controller_log (
      tw_controller_id_type     controller_id,
      tw_diagnostic_log_type*   controller_log
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the size of the specified 3ware controller's memory.
 *      The API verifies that the controller ID is valid, then requests
 *      the memory size from the specified controller.  The memory size
 *      of the specified controller is written to the location specified
 *      by the memory size parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      memory_size         - The address where tw_get_memory_size will store
 *                              the controller memory size.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The memory size for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The memory size for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have memory used for various controller functions.
 *      This routine provides the size of a particular controller's memory
 *      part in bytes.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_memory_size (
      tw_controller_id_type   controller_id,
      tw_memory_size_type*    memory_size
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the rebuild policy of the specified controller.  The API
 *      verifies the controller ID, then requests the rebuild policy from
 *      the specified controller.  The rebuild policy of the specified
 *      controller is written to the location specified by the
 *      auto_rebuild_policy parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      auto_rebuild_policy - The address where tw_get_auto_rebuild_policy
 *                              will store the rebuild policy.
 *                              Value will be 
 *                                TW_POLICY_AUTO_REBUILD_OFF,
 *                                TW_POLICY_AUTO_REBUILD_ALL
 *                                   or a bitwise OR'ed combination of:
 *                                TW_POLICY_AUTO_REBUILD_CBOD
 *                                TW_POLICY_AUTO_REBUILD_JBOD
 *                                TW_POLICY_AUTO_REBUILD_FAILED_DISK
 *
 *                            NOTE: TW_POLICY_AUTO_REBUILD_ALL includes 5 flags
 *                                    that are not presently defined.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The rebuild policy for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The rebuild policy for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the rebuild policy on a particular controller.
 *      A value of TW_POLICY_AUTO_REBUILD_OFF indicates that the controller
 *      will NOT start a rebuild operation when a unit, with a redundant
 *      configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_CBOD indicates that the controller
 *      will start a rebuild operation, using any available CBOD, when a unit,
 *      with a redundant configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_JBOD indicates that the controller
 *      will start a rebuild operation, using any available JBOD, when a unit,
 *      with a redundant configuration, degrades if the controller's JBOD
 *      policy is disabled (TW_POLICY_DISABLED).
 *
 *      A value of TW_POLICY_AUTO_REBUILD_FAILED_DISK indicates that the
 *      will start a rebuild operation, using the original failing disk,
 *      when a unit, with a redundant configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_ALL indicates that the controller
 *      will rebuild using all of the available options listed above.
 *
 *    See also:
 *      tw_set_auto_rebuild_policy
 *      tw_get_jbod_policy
 *      tw_disable_jbod_online
 *      tw_enable_jbod_online
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_auto_rebuild_policy (
          tw_controller_id_type          controller_id,
          tw_auto_rebuild_policy_type*   auto_rebuild_policy
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the LUN policy of the specified controller.  The API
 *      verifies the controller ID, then requests the LUN policy from
 *      the specified controller.  The LUN policy of the specified
 *      controller is written to the location specified by the
 *      lun_policy parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      lun_policy          - The address where tw_get_lun_policy
 *                              will store the LUN policy.
 *                              Value will be one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The LUN policy for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The LUN policy for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the LUN policy on a particular controller.
 *      A value of TW_POLICY_DISABLED indicates that each unit has exactly
 *      one LUN, lun 0.  A value of TW_POLICY_ENABLED indicates that the
 *      controller will create multiple LUNs per unit.
 *
 *    See also:
 *      tw_enable_lun_policy
 *      tw_disable_lun_policy
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_lun_policy (
      tw_controller_id_type   controller_id,
      tw_policy_state_type*   lun_policy
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the lun size of the specified controller.  The API verifies
 *        that the controller ID is valid, then requests the lun size from the
 *        specified controller.  The lun size of the controller is written to
 *        the location specified by the lun_size parameter.
 *
 *      Parameters:
 *        controller_id     - ID of the controller in question.
 *        lun_size          - The address where tw_get_unit_lun size
 *                              will store the unit lun size.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The lun size for the specified controller
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The lun size for the specified controller
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each controller has a lun size which is used during unit creation
 *        If the lun policy is enabled.  See tw_enable_lun_policy for more
 *        information.  Applications can use this routine to read a particular
 *        controller's lun size.
 *
 *      See also:
 *        tw_set_lun_size
 *        tw_get_lun_policy 
 *        tw_enable_lun_policy
 *        tw_disable_lun_policy
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_lun_size (
      tw_controller_id_type   controller_id,
      tw_lun_size_type*       lun_size
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the drive LBA reordering setting of the specified controller.
 *      The API verifies the controller ID, then requests the LBA reordering setting
 *      from the specified controller. The value is written to the location specified
 *      by the lba_reorder_state parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      lba_reorder_state   - The address where tw_get_lba_reorder_state
 *                              will store the value and will be one of:
 *                                TW_LBA_REORDER_DISABLED
 *                                TW_LBA_REORDER_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The LBA reordering setting for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The LBA reordering setting for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the drive LBA reordering setting on a particular controller.
 *      It is a controller level setting and the default is TW_LBA_REORDER_ENABLED.
 *      There are functions available to modify this setting. Refer to 'see also' section.
 *
 *    See also:
 *      tw_enable_lba_reorder
 *      tw_disable_lba_reorder
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_lba_reorder_state (
      tw_controller_id_type       controller_id,
      tw_lba_reorder_state_type*  lba_reorder_state
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the automatic drive detection policy on the specified port of the specified controller.
 *      The API verifies the controller ID, port ID specified. Then requests drive detection policy state
 *      from the specified controller. The value is written to the location specified by
 *      the spinup_state parameter.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      spinup_state        - The address where tw_get_auto_spinup_state
 *                              will store the value and will be one of:
 *                                TW_AUTO_SPINUP_DISABLED
 *                                TW_AUTO_SPINUP_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The drive detection policy state of the specified port
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The drive detection policy state of the specified port
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      The automatic drive detection policy is a per port level setting. Can also be set on group of ports
 *      of the same controller.
 *
 *    See also:
 *      tw_enable_auto_spinup
 *      tw_disable_auto_spinup
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_auto_spinup_state (
      tw_controller_id_type       controller_id,
      tw_port_id_type             port_id,
      tw_spinup_state_type*       spinup_state
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the maximum number of allowable units on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      maximum number of allowable units from the specified controller
 *      and the value is written to the location specified by the max_units parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      max_units           - The address where tw_get_max_units_supported will
 *                              store the maximum number of allowable units
 *                              on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The maximum number of allowable units
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The maximum number of allowable units 
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine get the maximum number of storage units allowed to create on a
 *      particular controller. Storage units, or "units" are the lowest
 *      level mass storage object visible to the operating system (OS).
 *      A five drive RAID 0 unit is looks like a large "disk drive" to the OS.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_max_units_supported (
      tw_controller_id_type   controller_id,
      int*                    max_units
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the maximum number of drives that can be connected on the specified
 *      3ware controller. The API verifies that the controller ID is valid,
 *      then requests the maximum number of drives from the specified controller
 *      and the value is written to the location specified by the max_drives parameter.
 *
 *    Parameters:
 *      controller_id    - ID of the controller in question.
 *      max_drives       - The address where tw_get_max_drives_supported will
 *                           store the maximum number of drives
 *                           on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The maximum number of drives that can be connected to a controller
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The maximum number of drives that can be connected to a controller
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_max_drives_supported (
      tw_controller_id_type   controller_id,
      int*                    max_drives
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the maximum number of allowable active drives on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      maximum number of allowable active drives from the specified controller
 *      and the value is written to the location specified by the max active drives parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      max_active_drives   - The address where tw_get_max_active_drives will
 *                              store the maximum number of allowable active drives
 *                              on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The maximum number of allowable active drives 
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The maximum number of allowable active drives 
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine get the maximum number of drives that can be active on a
 *      particular controller.
 *
 *      What is active Drive?
 *      A drive belongs to an unit that is online.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_max_active_drives (
      tw_controller_id_type   controller_id,
      int*                    max_active_drives
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the maximum number of allowable active units on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      maximum number of allowable active units from the specified controller
 *      and the value is written to the location specified by the max_active_units parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      max_active_units    - The address where tw_get_max_active_units will
 *                              store the maximum number of allowable active units
 *                              on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS     - The maximum number of allowable active units
 *                             is available in the specified location.
 *      TW_RET_FAILED      - The maximum number of allowable active units
 *                             is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine get the maximum number of units that can be active on a
 *      particular controller.
 *
 *      What is active unit?
 *      A unit that is online.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_max_active_units (
      tw_controller_id_type   controller_id,
      int*                    max_active_units
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the maximum number of allowable drives per RAID unit.
 *      The API verifies that the controller ID is valid, then requests the
 *      maximum number of allowable drives per RAID unit from the specified controller
 *      and the value is written to the location specified by the max_drives_per_unit parameter.
 *
 *    Parameters:
 *      controller_id         - ID of the controller in question.
 *      max_drives_per_unit   - The address where tw_get_max_drives_per_unit will
 *                                store the maximum number of allowable drives per RAID unit
 *                                on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The maximum number of allowable drives per RAID unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The maximum number of allowable drives per RAID unit
 *                            is NOT available in the specified location.
 * 
 *    Remarks:
 *      Use this routine read the maximum number of allowable drives per RAID unit on a
 *      particular controller.
 *
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_max_drives_per_unit (
      tw_controller_id_type   controller_id,
      int*                    max_drives_per_unit
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of active drives on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of active drives from the specified controller
 *      and the value is written to the location specified by the active drives parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      active_drives       - The address where tw_get_number_of_active_drives will
 *                              store the number of active drives
 *                              on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of active drives 
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The number of active drives
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine get the number of drives that are active on a
 *      particular controller.
 *
 *      What is active Drive?
 *      A drive belongs to an unit that is online.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_number_of_units
 *      tw_get_number_of_drives
 *      tw_get_max_active_drives
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_active_drives (
      tw_controller_id_type   controller_id,
      int*                    active_drives
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the drive coercion factor of the specified controller.
 *        The API verifies that the controller ID is valid, then requests the
 *        drive coercion factor from the specified controller. The drive coercion
 *        factor is written to the location specified by the coercion_factor parameter.
 *
 *      Parameters:
 *        controller_id     - ID of the controller in question.
 *        coercion_factor    - The address where tw_get_drive_coercion_factor
 *                              will store the value.                         
 *                              The drive coercion factor is in multiple of GB. i.e. 
 *                                   Drive coercion factor 1  --> 1 GB. 
 *                                   Drive coercion factor 2  --> 2 GB. 
 *                                   Drive coercion factor 3  --> 3 GB. and so on
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The drive coercion factor for the specified controller
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The drive coercion factor for the specified controller
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each controller has a drive coercion factor which is used during unit creation.
 *        The raw capacity of the drive is truncated to the smallest number of sectors that
 *        is larger than or equal to an even multiple of drive coersion factor.
 *        The controller default setting is 5 GB for a drive with raw capacity >= 45 GB.
 *                                          1 GB for a drive with raw capacity <  45 GB.
 *
 *      See also:
 *        tw_set_drive_coercion_factor
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_coercion_factor (
      tw_controller_id_type        controller_id,
      tw_coercion_factor_type*     coercion_factor
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the SCSI mode page control setting of the specified controller.
 *
 *    Parameters:
 *      controller_id        - ID of the controller in question.
 *      mode_control         - The address where tw_get_scsi_modepage_control
 *                               will store the SCSI mode page control value.
 *                                This value will be one of the following:
 *                                 TW_SCSI_MODEPAGE_CONTROL_HONOR
 *                                 TW_SCSI_MODEPAGE_CONTROL_IGNORE_WCE
 *
 *    Remarks:
 *      Use this routine to read the SCSI mode page control setting of
 *      a particular controller. This setting modifies the firmware behavior of
 *      host SCSI mode sense & mode select commands.  
 *
 *      A value of TW_SCSI_MODEPAGE_CONTROL_IGNORE_WCE indicates that the controller firmware
 *      ignores OS request via the SCSI Mode Select command to disable/enable write cache.
 *      This means write cache setting is only changeable through unit cache configuration routines.
 *      The complement of this is TW_SCSI_MODEPAGE_CONTROL_HONOR.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The SCSI mode page control setting of the specified controller
 *                           is available in the specified location.
 *      TW_RET_FAILED    - TheSCSI mode page control setting of the specified controller
 *                           is NOT available in the specified location.
 *
 *    See also:
 *      tw_set_scsi_modepage_control
 *      tw_enable_unit_write_cache
 *      tw_disable_unit_write_cache
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_scsi_modepage_control (
      tw_controller_id_type        controller_id,
      tw_modepage_control_type*    mode_control
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Provides the port handle to drive handle mapping of the specified controller.
 *
 *    Parameters:
 *      controller_id        - ID of the controller in question.
 *      handle_map           - The address where tw_get_port_to_drivehandle_map
 *                               will store the port to drive handle mapping.
 *
 *    Remarks:
 *      Use this routine to read the port handle to drive handle mapping of
 *      a particular controller.
 *      The handle map array returned is a 8-bit unsigned value of 255 entries.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The port handle to drive handle map of the specified controller
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The port handle to drive handle map of the specified controller
 *                           is NOT available in the specified location.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_to_drivehandle_map (
      tw_controller_id_type              controller_id,
      tw_port_to_drivehandle_map_type*   handle_map
      );


/**
 * <pre>
 *  Asynchronous Events
 *      3ware controllers log events asynchronously to the application.
 *  The API provides the application access to these asynchronous events.
 *  The following API routines specify the access interface to the 3ware
 *  controller asynchronous events.
 *
 *
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Retrieves the first event, if any, from the event queue.  When
 *      iterating all events, call this function to get the first event
 *      in the event queue.  Use the sequence_id in the tw_event_info
 *      structure in subsequent calls to tw_get_next_event.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      event_info        - The address where tw_get_first_event will
 *                            will store the event information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The event information is available in
 *                            the specified location.
 *      TW_RET_FAILED     - The event information is NOT available in
 *                            the specified location.
 *
 *    Remarks:
 *      Use this routine to read the first event of a particular controller's
 *      asynchronous event log.  This log is used to report asynchronous events
 *      such as unit degrade, rebuild task start and rebuild task stop.
 *      Note this routine performs a destructive (one time only) read of the
 *      asynchronous events from 7000 and 8000 series controllers.
 *      Event information is stored in a struct of type tw_event_info_type.
 *      The following fields within the error struct are of interest:
 *        sequence_id        - This is used while iterating through the events.
 *                               Use the sequence_id returned by AEN API call
 *                               to get next/previous event. For more information and
 *                               its use, see the description of tw_get_next_event() or
 *                               tw_get_previous_event().
 *        time_stamp_sec     - The system time when AEN occurred.
 *        aen_code           - A 16-bit hex value indicating the specific AEN.
 *        severity           - Indicates severity level of AEN,
 *                               This value will be one of the following:
 *                                 TW_AEN_SEVERITY_ERROR
 *                                 TW_AEN_SEVERITY_WARNING
 *                                 TW_AEN_SEVERITY_INFORMATION
 *                                 TW_AEN_SEVERITY_DEBUG
 *        reserved0          - Reserved.
 *        reserved1          - Reserved.
 *        parameter_len      - Length of the text in 'parameter_data' field not including
 *                               the terminating \0.
 *        parameter_data     - Short description of AEN in ASCII text.
 *        text_len           - Length of the text in 'text' field not including
 *                               the terminating \0.
 *        text               - Detailed description of AEN in ASCII text.
 *
 *    See also:
 *      tw_get_last_event
 *      tw_get_next_event
 *      tw_get_previous_event
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_first_event (
      tw_controller_id_type   controller_id,
      tw_event_info_type*     event_info
                       );
 
 
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Retrieves the last event, if any, from the event queue.  When
 *      iterating all events, call this function to get the last event
 *      in the event queue.  Use the sequence_id in the tw_event_info
 *      structure in subsequent calls to tw_get_previous_event.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      event_info        - The address where tw_get_last_event will
 *                            will store the event information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The event information is available in
 *                            the specified location.
 *      TW_RET_FAILED     - The event information is NOT available in
 *                            the specified location.
 *
 *    Remarks:
 *      Use this routine to read the last event of a particular controller's
 *      asynchronous event log.  This log is used to report asynchronous events
 *      such as unit degrade, rebuild task start and rebuild task stop.
 *      Event information is stored in a struct of type tw_event_info_type.
 *      The following fields within the error struct are of interest:
 *        sequence_id        - This is used while iterating through the events.
 *                               Use the sequence_id returned by AEN API call
 *                               to get next/previous event. For more information and
 *                               its use, see the description of tw_get_next_event() or
 *                               tw_get_previous_event().
 *        time_stamp_sec     - The system time when AEN occurred.
 *        aen_code           - A 16-bit hex value indicating the specific AEN.
 *        severity           - Indicates severity level of AEN,
 *                               This value will be one of the following:
 *                                 TW_AEN_SEVERITY_ERROR
 *                                 TW_AEN_SEVERITY_WARNING
 *                                 TW_AEN_SEVERITY_INFORMATION
 *                                 TW_AEN_SEVERITY_DEBUG
 *        reserved0          - Reserved.
 *        reserved1          - Reserved.
 *        parameter_len      - Length of the text in 'parameter_data' field not including
 *                               the terminating \0.
 *        parameter_data     - Short description of AEN in ASCII text.
 *        text_len           - Length of the text in 'text' field not including
 *                               the terminating \0.
 *        text               - Detailed description of AEN in ASCII text.
 *
 *    See also:
 *      tw_get_first_event
 *      tw_get_next_event
 *      tw_get_previous_event
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_last_event (
      tw_controller_id_type   controller_id,
      tw_event_info_type*     event_info
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Retrieves the next event, if any, from the event queue
 *      relative to the sequence ID.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      sequence_id       - Sequence ID of the last event retrieved.
 *      event_info        - The address where tw_get_next_event will
 *                            will store the event information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The event information is available in
 *                            the specified location.
 *      TW_RET_FAILED     - The event information is NOT available in
 *                            the specified location.
 *
 *    Remarks:
 *      Use this routine to read the next event of a particular controller's
 *      asynchronous event log.  This log is used to report asynchronous events
 *      such as unit degrade, rebuild task start and rebuild task stop.
 *      Note this routine performs a destructive (one time only) read of the
 *      asynchronous events from 7000 and 8000 series controllers.
 *      Event information is stored in a struct of type tw_event_info_type.
 *      The following fields within the error struct are of interest:
 *        sequence_id        - This is used while iterating through the events.
 *                               Use the sequence_id returned by AEN API call
 *                               to get next/previous event. For more information and
 *                               its use, see the description of tw_get_next_event() or
 *                               tw_get_previous_event().
 *        time_stamp_sec     - The system time when AEN occurred.
 *        aen_code           - A 16-bit hex value indicating the specific AEN.
 *        severity           - Indicates severity level of AEN,
 *                               This value will be one of the following:
 *                                 TW_AEN_SEVERITY_ERROR
 *                                 TW_AEN_SEVERITY_WARNING
 *                                 TW_AEN_SEVERITY_INFORMATION
 *                                 TW_AEN_SEVERITY_DEBUG
 *        reserved0          - Reserved.
 *        reserved1          - Reserved.
 *        parameter_len      - Length of the text in 'parameter_data' field not including
 *                               the terminating \0.
 *        parameter_data     - Short description of AEN in ASCII text.
 *        text_len           - Length of the text in 'text' field not including
 *                               the terminating \0.
 *        text               - Detailed description of AEN in ASCII text.
 *
 *    See also:
 *      tw_get_first_event
 *      tw_get_last_event
 *      tw_get_previous_event
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_next_event (
      tw_controller_id_type    controller_id,
      tw_event_sequence_type   sequence_id,
      tw_event_info_type*      event_info
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Retrieves the previous event, if any, from the event queue
 *      relative to the sequence ID.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      sequence_id       - Sequence ID of the last event retrieved.
 *      event_info        - The address where tw_get_previous_event will
 *                            will store the event information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The event information is available in
 *                            the specified location.
 *      TW_RET_FAILED     - The event information is NOT available in
 *                            the specified location.
 *
 *    Remarks:
 *      Use this routine to read the previous event of a particular controller's
 *      asynchronous event log.  This log is used to report asynchronous events
 *      such as unit degrade, rebuild task start and rebuild task stop.
 *      Event information is stored in a struct of type tw_event_info_type.
 *      The following fields within the error struct are of interest:
 *        sequence_id        - This is used while iterating through the events.
 *                               Use the sequence_id returned by AEN API call
 *                               to get next/previous event. For more information and
 *                               its use, see the description of tw_get_next_event() or
 *                               tw_get_previous_event().
 *        time_stamp_sec     - The system time when AEN occurred.
 *        aen_code           - A 16-bit hex value indicating the specific AEN.
 *        severity           - Indicates severity level of AEN,
 *                               This value will be one of the following:
 *                                 TW_AEN_SEVERITY_ERROR
 *                                 TW_AEN_SEVERITY_WARNING
 *                                 TW_AEN_SEVERITY_INFORMATION
 *                                 TW_AEN_SEVERITY_DEBUG
 *        reserved0          - Reserved.
 *        reserved1          - Reserved.
 *        parameter_len      - Length of the text in 'parameter_data' field not including
 *                               the terminating \0.
 *        parameter_data     - Short description of AEN in ASCII text.
 *        text_len           - Length of the text in 'text' field not including
 *                               the terminating \0.
 *        text               - Detailed description of AEN in ASCII text.
 *
 *    See also:
 *      tw_get_first_event
 *      tw_get_last_event
 *      tw_get_next_event
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_previous_event (
      tw_controller_id_type    controller_id,
      tw_event_sequence_type   sequence_id,
      tw_event_info_type*      event_info
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Indicates whether the battery backup unit is present on the
 *      specified controller.  The API verifies that the controller ID
 *      is valid, then requests if the battery backup unit is present.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bbu_present         - The address where tw_bbu_is_present will
 *                              store the result.  Returns one of:
 *                                TW_BBU_IS_NOT_PRESENT
 *                                TW_BBU_IS_PRESENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully determined the BBU existence on
 *                              the specified controller.
 *      TW_RET_FAILED       - Failed to determine the BBU existence on
 *                              the specified controller.
 *
 *    Remarks:
 *      Use this routine to determine if a particular controller has a
 *      battery backup unit.
 *
 *    See also:
 *      tw_bbu_battery_is_present
 *      tw_initialize_battery
 *      tw_test_battery
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_is_present (
      tw_controller_id_type     controller_id,
      tw_bbu_is_present_type*   bbu_present
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Indicates whether the battery on the battery backup unit is present
 *      on the specified controller.  The API verifies that the controller ID
 *      is valid, then requests if the battery is present.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bbu_battery_present - The address where tw_bbu_battery_is_present
 *                              will store the result.  Returns one of:
 *                                TW_BBU_BATTERY_IS_NOT_PRESENT
 *                                TW_BBU_BATTERY_IS_PRESENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully determined the battery existence on
 *                              the specified controller's battery backup unit.
 *      TW_RET_FAILED       - Failed to determine the battery existence on
 *                              the specified controller's battery backup unit.
 *
 *    Remarks:
 *      Use this routine to determine if a particular controller has a
 *      battery installed on its battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_initialize_battery
 *      tw_test_battery
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_battery_is_present (
      tw_controller_id_type             controller_id,
      tw_bbu_battery_is_present_type*   bbu_battery_present
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the status of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid
 *      and the battery backup unit is present, then requests the status of
 *      the battery backup unit from the specified controller.
 *      The status of the battery backup unit is written to the location
 *      specified by the bbu_status parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bbu_status          - The address where tw_get_bbu_status
 *                              will store the result.  Returns one of:
 *                                TW_BBU_STATUS_NOT_READY
 *                                TW_BBU_STATUS_READY
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU status.
 *      TW_RET_FAILED       - Failed to get the BBU status.
 *
 *    Remarks:
 *      Use this routine to read the status of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_status (
      tw_controller_id_type   controller_id,
      tw_bbu_status_type*     bbu_status
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery voltage status of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      battery voltage status of the battery backup unit from the specified
 *      controller.  The battery voltage status of the battery backup unit is
 *      written to the location specified by the
 *        bbu_battery_voltage_status parameter.
 *
 *    Parameters:
 *      controller_id              - ID of the controller in question.
 *      bbu_battery_voltage_status - The address where
 *                                     tw_get_bbu_battery_voltage_status
 *                                     will store the result.
 *                                     Returns one of:
 *                                       TW_BBU_BATTERY_VOLTAGE_TOO_LOW
 *                                       TW_BBU_BATTERY_VOLTAGE_LOW
 *                                       TW_BBU_BATTERY_VOLTAGE_NORMAL
 *                                       TW_BBU_BATTERY_VOLTAGE_HIGH
 *                                       TW_BBU_BATTERY_VOLTAGE_TOO_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully read the BBU battery voltage status.
 *      TW_RET_FAILED       - Failed to read the BBU battery voltage status.
 *
 *    Remarks:
 *      Use this routine to read the battery voltage status of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_bbu_battery_temperature_status
 *      tw_get_bbu_battery_temperature
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_voltage_status (
      tw_controller_id_type                 controller_id,
      tw_bbu_battery_voltage_status_type*   bbu_battery_voltage_status
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery temperature status of the battery backup unit on
 *      the specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      battery temperature status of the battery backup unit from the specified
 *      controller.  The battery temperature status of the battery backup unit
 *      is written to the location specified by the
 *        bbu_battery_temperature_status parameter.
 *
 *    Parameters:
 *      controller_id                  - ID of the controller in question.
 *      bbu_battery_temperature_status - The address where
 *                                         tw_get_bbu_battery_temperature_status
 *                                         will store the result.
 *                                         Returns one of:
 *                                           TW_BBU_BATTERY_TEMPERATURE_TOO_LOW
 *                                           TW_BBU_BATTERY_TEMPERATURE_LOW
 *                                           TW_BBU_BATTERY_TEMPERATURE_NORMAL
 *                                           TW_BBU_BATTERY_TEMPERATURE_HIGH
 *                                           TW_BBU_BATTERY_TEMPERATURE_TOO_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - Successfully read the BBU battery temperature status.
 *      TW_RET_FAILED    - Failed to read the BBU battery temperature status.
 *
 *    Remarks:
 *      Use this routine to read the battery temperature status of a particular
 *      controller's battery backup unit. To to read the battery temperature
 *      in degree Celsius see tw_get_bbu_battery_temperature() routine.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_bbu_battery_voltage_status
 *      tw_get_bbu_battery_temperature
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_temperature_status (
      tw_controller_id_type                     controller_id,
      tw_bbu_battery_temperature_status_type*   bbu_battery_temperature_status
                                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery temperature(in degree Celsius) of the battery backup unit on
 *      the specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      battery temperature of the BBU from the specified controller.
 *      The battery temperature value is written to the location specified by the
 *      temperature parameter.
 *
 *    Parameters:
 *      controller_id                  - ID of the controller in question.
 *      temperature                    - The address where
 *                                         tw_get_bbu_battery_temperature
 *                                         will store the temperature value.
 *    Return Values:
 *      TW_RET_SUCCESS   - Successfully read the BBU battery temperature.
 *      TW_RET_FAILED    - Failed to read the BBU battery temperature.
 *
 *    Remarks:
 *      Use this routine to read the battery temperature in degree Celsius of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_bbu_battery_temperature_status
 *      tw_get_bbu_battery_voltage_status
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_temperature (
      tw_controller_id_type     controller_id,
      int*                      temperature
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Indicates whether the battery on the battery backup unit, on the
 *      specified controller is being tested.  The API verifies that the
 *      controller ID is valid, then requests if the battery is being tested.
 *
 *    Parameters:
 *      controller_id                 - ID of the controller in question.
 *      bbu_battery_test_in_progress  - The address where
 *                                        tw_bbu_battery_test_in_progress
 *                                        will store the result.
 *                                        Returns one of:
 *                                           TW_BBU_BATTERY_IS_NOT_TESTING
 *                                           TW_BBU_BATTERY_IS_TESTING
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully determined if the battery on the
 *                              specified controller's battery backup unit
 *                              is being tested.
 *      TW_RET_FAILED       - Failed to determine if the battery on the
 *                              specified controller's battery backup unit
 *                              is being tested.
 *
 *    Remarks:
 *      Use this routine to find out if the battery on a particular
 *      controller's battery backup unit is being tested.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_battery_test_in_progress (
      tw_controller_id_type                   controller_id,
      tw_bbu_battery_test_in_progress_type*   bbu_battery_test_in_progress
                                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Indicates whether the battery on the battery backup unit, on the
 *      specified controller is being charged.  The API verifies that the
 *      controller ID is valid, then requests if the battery is being charged.
 *
 *    Parameters:
 *      controller_id                   - ID of the controller in question.
 *      bbu_battery_charge_in_progress  - The address where
 *                                          tw_bbu_battery_charge_in_progress
 *                                          will store the result.
 *                                          Returns one of:
 *                                            TW_BBU_BATTERY_IS_NOT_CHARGING
 *                                            TW_BBU_BATTERY_IS_CHARGING
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully determined if the battery on the
 *                              specified controller's battery backup unit
 *                              is being charged.
 *      TW_RET_FAILED       - Failed to determine if the battery on the
 *                              specified controller's battery backup unit
 *                              is being charged.
 *
 *    Remarks:
 *      Use this routine to find out if the battery on a particular
 *      controller's battery backup unit is being charged.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_battery_charge_in_progress (
      tw_controller_id_type                     controller_id,
      tw_bbu_battery_charge_in_progress_type*   bbu_battery_charge_in_progress
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the serial number of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid and
 *      the battery backup unit is present, then requests the serial number
 *      of the battery backup unit from the specified controller.  
 *      The serial number of the battery backup unit is written to the location
 *      specified by the bbu_serial_number parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bbu_serial_number   - The address where tw_get_bbu_serial_number
 *                              will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU serial number.
 *      TW_RET_FAILED       - Failed to get the BBU serial number.
 *
 *    Remarks:
 *      Use this routine to read the serial number of a particular controller's
 *      battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_serial_number (
      tw_controller_id_type        controller_id,
      tw_bbu_serial_number_type*   bbu_serial_number
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the firmware version of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      firmware version of the battery backup unit from the specified
 *      controller.
 *      The firmware version of the battery backup unit is written to the
 *      location specified by the bbu_firmware_version parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      bbu_firmware_version     - The address where
 *                                   tw_get_bbu_firmware_version
 *                                   will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU firmware version.
 *      TW_RET_FAILED       - Failed to get the BBU firmware version.
 *
 *    Remarks:
 *      Use this routine to read the firmware version of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_firmware_version (
      tw_controller_id_type           controller_id,
      tw_bbu_firmware_version_type*   bbu_firmware_version
                                );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the boot code version of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      boot code version of the battery backup unit from the specified
 *      controller.
 *      The boot code version of the battery backup unit is written to the
 *      location specified by the bbu_boot_code_version parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      bbu_boot_code_version    - The address where
 *                                   tw_get_bbu_boot_code_version
 *                                   will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU boot code version.
 *      TW_RET_FAILED       - Failed to get the BBU boot code version.
 *
 *    Remarks:
 *      Use this routine to read the boot code version of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_boot_code_version (
      tw_controller_id_type            controller_id,
      tw_bbu_boot_code_version_type*   bbu_boot_code_version
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the PCB revision of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid and
 *      the battery backup unit is present, then requests the PCB revision of
 *      the battery backup unit from the specified controller.
 *      The PCB revision of the battery backup unit is written to the location
 *      specified by the bbu_pcb_revision parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      bbu_pcb_revision         - The address where
 *                                   tw_get_bbu_pcb_revision
 *                                   will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU PCB revision.
 *      TW_RET_FAILED       - Failed to get the BBU PCB revision.
 *
 *    Remarks:
 *      Use this routine to read the PCB revision of a particular controller's
 *      battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_pcb_revision (
      tw_controller_id_type       controller_id,
      tw_bbu_pcb_revision_type*   bbu_pcb_revision
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the factory data of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, then requests the
 *      factory data of the battery backup unit from the specified
 *      controller.
 *      The factory data of the battery backup unit is written to the
 *      location specified by the bbu_factory_data parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      bbu_factory_data         - The address where
 *                                   tw_get_bbu_factory_data
 *                                   will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU factory data.
 *      TW_RET_FAILED       - Failed to get the BBU factory data.
 *
 *    Remarks:
 *      Use this routine to read the factory data of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_factory_data (
      tw_controller_id_type       controller_id,
      tw_bbu_factory_data_type*   bbu_factory_data
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery installation date of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID is
 *      valid and the battery backup unit is present, the battery is present,
 *      then requests the battery installation date of the battery backup unit
 *      from the specified controller.
 *      The battery installation date of the battery backup unit is written to
 *      the location specified by the bbu_battery_installation_date parameter.
 *
 *    Parameters:
 *      controller_id                   - ID of the controller in question.
 *      bbu_battery_installation_date   - The address where
 *                                          tw_get_bbu_battery_installation_date
 *                                          will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS     - Successfully got the BBU battery installation date.
 *      TW_RET_FAILED      - Failed to get the BBU battery installation date.
 *
 *    Remarks:
 *      Use this routine to read the installation date of a particular
 *      controller's BBU battery.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_installation_date (
      tw_controller_id_type   controller_id,
      tw_bbu_date_type*       bbu_battery_installation_date
                                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the last battery test date of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID is
 *      valid and the battery backup unit is present, the battery is present,
 *      then requests the last battery test date of the battery backup unit
 *      from the specified controller.
 *      The last battery test date of the battery backup unit is written to
 *      the location specified by the bbu_battery_test_date parameter.
 *
 *    Parameters:
 *      controller_id            - ID of the controller in question.
 *      bbu_battery_test_date    - The address where
 *                                   tw_get_bbu_last_battery_test_date
 *                                   will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU last battery test date.
 *      TW_RET_FAILED       - Failed to get the BBU last battery test date.
 *
 *    Remarks:
 *      Use this routine to read the last battery test date of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_last_battery_test_date (
      tw_controller_id_type   controller_id,
      tw_bbu_date_type*       bbu_battery_test_date
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery capacity of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID
 *      is valid and the battery backup unit is present, the battery is present,
 *      then requests the battery capacity of the battery backup unit from the
 *      specified controller.
 *      The battery capacity of the battery backup unit is written to
 *      the location specified by the bbu_battery_capacity parameter.
 *
 *    Parameters:
 *      controller_id               - ID of the controller in question.
 *      bbu_battery_capacity        - The address where
 *                                      tw_get_bbu_battery_capacity
 *                                      will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU battery capacity.
 *      TW_RET_FAILED       - Failed to get the BBU battery capacity.
 *
 *    Remarks:
 *      Use this routine to read the battery capacity of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_capacity (
      tw_controller_id_type           controller_id,
      tw_bbu_battery_capacity_type*   bbu_battery_capacity
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery charge voltage of the battery backup unit on the
 *      specified 3ware controller.  The API verifies that the controller ID is
 *      valid, the battery backup unit is present, the battery is present, then
 *      requests the battery charge voltage of the battery backup unit from the
 *      specified controller.  The battery charge voltage of the battery backup
 *      unit is written to the location specified by the
 *      charge_voltage parameter.
 *
 *    Parameters:
 *      controller_id              - ID of the controller in question.
 *      charge_voltage             - The address where
 *                                     tw_get_bbu_battery_charge_voltage
 *                                     will store the value will be one of
 *                                       TW_BBU_BATTERY_VOLTAGE_LOW_CHARGE
 *                                       TW_BBU_BATTERY_VOLTAGE_FULL_CHARGE
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU battery charge voltage.
 *      TW_RET_FAILED       - Failed to get the BBU battery charge voltage.
 *
 *    Remarks:
 *      Use this routine to read the battery charge voltage of a particular
 *      controller's battery backup unit. The controller firmware default value is 
 *      set to LOW_CHARGE when the battery is installed and will be adjusted by
 *      the controller firmware automatically to extend battery life. 
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_charge_voltage (
      tw_controller_id_type                 controller_id,
      tw_bbu_battery_charge_voltage_type*   charge_voltage
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides a status summary of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid
 *      then requests the status summary of the battery backup unit from the
 *      specified controller.  The battery temperature status of the battery
 *      backup unit is written to the location specified by the
 *      bbu_status_summary parameter.
 *
 *    Parameters:
 *      controller_id                  - ID of the controller in question.
 *      bbu_status_summary             - The address where
 *                                         tw_get_bbu_status_summary
 *                                         will store the result.
 *                                         Returns one of:
 *                                           TW_BBU_SUMMARY_OK
 *                                           TW_BBU_SUMMARY_BBU_NOT_PRESENT
 *                                           TW_BBU_SUMMARY_NO_BATTERY
 *                                           TW_BBU_SUMMARY_TESTING
 *                                           TW_BBU_SUMMARY_CHARGING
 *                                           TW_BBU_SUMMARY_FAULT
 *                                           TW_BBU_SUMMARY_ERROR
 *                                           TW_BBU_SUMMARY_WEAK_BATTERY
 *                                           TW_BBU_SUMMARY_FAILED_BATTERY
 *                                           TW_BBU_SUMMARY_DISABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - Successfully read the BBU status summary.
 *      TW_RET_FAILED    - Failed to read the BBU status summary.
 *
 *    Remarks:
 *      Use this routine to read the status summary of a particular
 *      controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_bbu_battery_is_present
 *      tw_get_bbu_status
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_status_summary (
      tw_controller_id_type         controller_id,
      tw_bbu_status_summary_type*   bbu_status_summary
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the online state of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid
 *      and the battery backup unit is present, then requests the online state
 *      of the battery backup unit from the specified controller.
 *      The online state of the battery backup unit is written to the location
 *      specified by the bbu_online_state parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      bbu_online_state    - The address where tw_get_bbu_online_state
 *                              will store the result.  Returns one of:
 *                                TW_BBU_IS_ENABLED
 *                                TW_BBU_IS_DISABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU online state.
 *      TW_RET_FAILED       - Failed to get the BBU online state.
 *
 *    Remarks:
 *      Use this routine to read the online state of a particular controller's
 *      battery backup unit.  A disabled BBU will not be used by the controller
 *      in the event of a power failure.
 *
 *    See also:
 *      tw_bbu_enable
 *      tw_bbu_disable
 *      tw_get_bbu_status_summary
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_online_state (
      tw_controller_id_type   controller_id,
      tw_bbu_enabled_type*    bbu_online_state
                            );



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the count of completed charging cycles of the battery backup unit on the specified
 *      3ware controller.  The API verifies that the controller ID is valid and
 *      the battery backup unit is present before sending request to the specified controller.  
 *      The charging cycle count of the battery backup unit is written to the location
 *      specified by the cycle_count parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      cycle_count         - The address where tw_get_bbu_charge_cycle_count
 *                              will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the BBU charge cycle count.
 *      TW_RET_FAILED       - Failed to get the BBU charge cycle count.
 *                             see tw_get_error_information() for getting details on error.
 *
 *    Remarks:
 *      Use this routine to read the count of completed charging cycles since the battery was installed,
 *      from a particular controller's battery backup unit.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_charge_cycle_count(
      tw_controller_id_type        controller_id,
      unsigned int*                cycle_count
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the current battery voltage of the battery backup unit on the
 *      specified 3ware controller.
 *
 *    Parameters:
 *      controller_id              - ID of the controller in question.
 *      battery_voltage            - The address where
 *                                     tw_get_bbu_battery_voltage
 *                                     will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfuly got the BBU battery voltage.
 *      TW_RET_FAILED       - Failed to get the BBU battery voltage.
 *
 *    Remarks:
 *      Use this routine to read the current battery voltage of a particular
 *      controller's battery backup unit. The battery voltage is in milivolts.
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_voltage (
      tw_controller_id_type          controller_id,
      tw_bbu_battery_voltage_type*   battery_voltage
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Provides the battery voltage at the time of high-current test of the battery.
 *
 *    Parameters:
 *      controller_id              - ID of the controller in question.
 *      battery_voltage            - The address where
 *                                     tw_get_bbu_battery_high_current_voltage
 *                                     will store the result.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfuly got the BBU battery voltage.
 *      TW_RET_FAILED       - Failed to get the BBU battery voltage.
 *
 *    Remarks:
 *      Use this routine to read the battery voltage of a particular
 *      controller's battery backup unit. The battery voltage is in milivolts
 *      measured at the time of high current battery test. 
 *
 *    See also:
 *      tw_bbu_is_present
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_bbu_battery_high_current_voltage (
      tw_controller_id_type          controller_id,
      tw_bbu_battery_voltage_type*   battery_voltage
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of phys on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of phys from the specified controller.  The number of phys
 *      on the specified controller is written to the location specified by
 *      the number_of_phys parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      number_of_phys      - The address where tw_get_number_of_phys will
 *                              store the number of phys on the specified
 *                              controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of phys for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The number of phys for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers have a fixed number of phys on each
 *      controller.  Use this routine to determine the number of phys
 *      on a particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_phys (
      tw_controller_id_type   controller_id,
      int*                    number_of_phys
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of phys from the specified controller.  The API
 *        verifies that the controller ID is valid, then requests the number
 *        and IDs of all phys from the specified controller.  The phy IDs
 *        are written to the location specified by phy_id_list.  The number
 *        of phys on the specified controller is written to the length
 *        element of the phy id list.
 *        
 *
 *      Parameters:
 *        controller_id     - ID of the controller in question.
 *        phy_id_list      - The address where tw_get_phy_id_list
 *                              will store the phy id list.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The phy id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The phy id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API that need
 *        phy related information must provide the ID of the phy of interest.
 *        This routine will provide a list of all phys, by ID, on a particular
 *        controller.  The phy ID list is stored in a struct of type
 *        tw_phy_id_list_type.  The following fields within the list are of
 *        interest:
 *             list                   - An array of <length> phy IDs.
 *             length                 - The number of phy IDs in the list.
 *
 *      See also:
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_id_list (
      tw_controller_id_type   controller_id,
      tw_phy_id_list_type*    phy_id_list
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *      Description:
 *        Provides the SAS address of the specified phy.
 *        The API verifies that the phy ID is valid, then requests the
 *        SAS address of the specified phy from the API.
 *        The SAS address is written to the location specified by the
 *        sas_address parameter.
 *
 *      Parameters:
 *        phy_id             - ID of the phy in question.
 *        sas_address        - The address where tw_get_sas_address
 *                                will store the SAS address.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The SAS address for the specified phy
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The SAS address for the specified phy
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to get the SAS address by a phy ID. 
 *
 *      See also:
 *        tw_create_phy_id
 *        tw_get_phy_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_sas_address (
      tw_phy_id_type         phy_id,
      tw_sas_address_type*   sas_address
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Provides the device type directly attached to the specified phy.
 *      The API verifies that the phy ID is valid, then requests the
 *      device type directly attached to the specified phy from the API.
 *      The device type is written to the location specified by the
 *      device_type parameter.
 *
 *    Parameters:
 *      phy_id             - ID of the phy in question.
 *      device_type        - The address where tw_get_device_type
 *                             will store the device type,
 *                             The value is one of the following:
 *                               TW_DEVICE_TYPE_NO_DEVICE
 *                               TW_DEVICE_TYPE_SATA
 *                               TW_DEVICE_TYPE_SAS
 *                               TW_DEVICE_TYPE_ENCLOSURE
 *                               TW_DEVICE_TYPE_ENCLOSURE_WITH_DISCOVERY_ERROR
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The device type for the specified phy
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The device type for the specified phy
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get the device type directly attached to controller phy,
 *      by a phy ID. 
 *
 *    See also:
 *      tw_create_phy_id
 *      tw_get_phy_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_device_type (
      tw_phy_id_type         phy_id,
      tw_device_type_type*   device_type
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides the phy link capability of the specified phy.
 *        The API verifies that the phy ID is valid, then requests the
 *        link capability of the specified phy from the phy's controller.
 *        The link capability of the phy is written to the location specified
 *        by the link_capability parameter.
 *
 *      Parameters:
 *        phy_id            - ID of the phy in question.
 *        link_capability   - The address where tw_get_phy_link_capability
 *                              will store the phy's link capability.
 *                              This is a bitwise OR'ed combination of
 *                              the following link flags:
 *                                TW_PHY_LINK_1_5_GBPS
 *                                TW_PHY_LINK_3_0_GBPS
 *                                TW_PHY_LINK_6_0_GBPS
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The link capability for the specified phy
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The link capability for the specified phy
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to read a particular phy's link capability.
 *          TW_PHY_LINK_1_5_GBPS - The phy supports a 1.5 Gbps link speed.
 *          TW_PHY_LINK_3_0_GBPS - The phy supports a 3.0 Gbps link speed.
 *          TW_PHY_LINK_6_0_GBPS - The phy supports a 6.0 Gbps link speed.
 *
 *      See also:
 *        tw_get_phy_link_status
 *        tw_get_phy_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_link_capability (
      tw_phy_id_type      phy_id,
      tw_phy_link_type*   link_capability
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides the phy link status of the specified phy.
 *        The API verifies that the phy ID is valid, then requests the
 *        link status of the specified phy from the phy's controller.
 *        The link status of the phy is written to the location specified
 *        by the link_status parameter.
 *
 *      Parameters:
 *        phy_id          - ID of the phy in question.
 *        link_status     - The address where tw_get_phy_link_status
 *                            will store the phy's link status.
 *                            The value is one of the following:
 *                              TW_PHY_NO_LINK
 *                              TW_PHY_LINK_1_5_GBPS
 *                              TW_PHY_LINK_3_0_GBPS
 *                              TW_PHY_LINK_6_0_GBPS
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The link status for the specified phy
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The link status for the specified phy
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to read a particular phy's link status.
 *          TW_PHY_LINK_1_5_GBPS - The phy supports a 1.5 Gbps link speed.
 *          TW_PHY_LINK_3_0_GBPS - The phy supports a 3.0 Gbps link speed.
 *          TW_PHY_LINK_6_0_GBPS - The phy supports a 6.0 Gbps link speed.
 *
 *      See also:
 *        tw_get_phy_link_capability
 *        tw_get_phy_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_link_status (
      tw_phy_id_type      phy_id,
      tw_phy_link_type*   link_status
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the current phy link negotiation limit setting on the specified phy.
 *      The API verifies that the phy ID is valid, then requests the
 *      link speed limit of the specified phy from the phy's controller.
 *      The link speed limit of the phy is written to the location specified
 *      by the link_limit parameter.
 *
 *    Parameters:
 *      phy_id          - ID of the phy in question.
 *      link_limit      - The address where tw_get_phy_link_negotiation_limit
 *                          will store the phy's link limit.
 *                          The value is one of the following:
 *                            TW_PHY_LINK_SPEED_AUTO
 *                            TW_PHY_LINK_SPEED_LIMIT_1_5_GBPS
 *                            TW_PHY_LINK_SPEED_LIMIT_3_0_GBPS
 *                            TW_PHY_LINK_SPEED_LIMIT_6_0_GBPS
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The link speed limit setting of the specified phy
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The link speed limit setting of the specified phy
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular phy's current limit on link speed negotiation.
 *      If the specified phy is part of SAS wide port then the slowest link speed setting
 *      will be applied to all phys that belong to that wide port.
 *
 *    See also:
 *      tw_set_phy_link_negotiation_limit
 *      tw_get_phy_link_capability
 *      tw_get_phy_link_status
 *      tw_get_phy_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_link_negotiation_limit (
      tw_phy_id_type      phy_id,
      tw_phy_link_type*   link_limit
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of enclosures in the system.
 *      The API probes the system to determine the number of enclosures
 *      associated with 3ware controllers, if any.  The number of enclosures
 *      in the system is written to the location specified by
 *      number_of_enclosures.
 *
 *    Parameters:
 *      number_of_enclosures  - The address where tw_get_number_of_enclosures
 *                              will store the number of enclosures in the
 *                              system.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of enclosures for the specified controller
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of enclosures for the specified controller
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Systems with 3ware controllers may have associated enclosures.
 *      This routine provides the number of enclosures associated with
 *      3ware controllers in a system.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_enclosures ( int*   number_of_enclosures );


/**
 * <pre>
 *  Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a list of the enclosures associated with 3ware controllers
 *      in the system.  The API probes the system to determine the number and
 *      IDs of enclosures, if any.  The IDs are written to the location
 *      specified by enclosure_id_list.  The number of enclosures in the system
 *      is written to the length element of the controller id list.
 *
 *    Parameters:
 *      enclosure_id_list - The address where tw_get_enclosure_id_list
 *                            will store the enclosure id list and length.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure id list and length are
 *                            available in the specified location.
 *      TW_RET_FAILED     - The enclosure id list is NOT valid and
 *                            the list length is set to zero.
 *
 *    Remarks:
 *      Applications that make calls to other routines in this API to get
 *      or set enclosure related information must provide the ID of the
 *      enclosure of interest.  This routine will provide a list of all
 *      enclosures, by ID.  The enclosure ID list is stored in a struct
 *      of type tw_enclosure_id_list_type.  The following fields within
 *      the list are of interest:
 *           list                   - An array of <length> enclosure IDs.
 *           length                 - The number of enclosure IDs in the list.
 *
 *    See also:
 *      tw_get_number_of_enclosures
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_enclosure_id_list ( tw_enclosure_id_list_type*   enclosure_id_list );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of enclosures on the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      number of enclosures from the specified controller.  The number of
 *      enclosures on the specified controller is written to the location
 *      specified by the number_of_enclosures parameter.
 *
 *    Parameters:
 *      controller_id         - ID of the controller in question.
 *      number_of_enclosures  - The address where this routine will store the
 *                              number of enclosures on the specified controller.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of enclosures for the specified controller
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of enclosures for the specified controller
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      3ware Controllers may have enclosures attached to each controller.
 *      This routine provides the number of enclosures attached to a
 *      particular controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_enclosure_id_list_on_controller
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_enclosures_on_controller (
      tw_controller_id_type   controller_id,
      int*                    number_of_enclosures
                                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of enclosures from the specified controller.
 *        The API verifies that the controller ID is valid, then requests
 *        the number and IDs of all enclosures from the specified controller.
 *        The enclosure IDs are written to the location specified by the
 *        enclosure_id_list parameter.  The number of enclosures on the
 *        specified controller is written to the length element of the
 *        enclosure id list.
 *
 *      Parameters:
 *        controller_id     - ID of the controller in question.
 *        enclosure_id_list - The address where this routine will store the
 *                              enclosure id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The enclosure id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The enclosure id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set enclosure related information must provide the ID of the
 *        enclosure of interest.  This routine will provide a list of all
 *        enclosures, by ID, on a particular controller.  The enclosure ID list
 *        is stored in a struct of type tw_enclosure_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> enclosure IDs.
 *             length                 - The number of enclosure IDs in the list.
 *
 *      See also:
 *        tw_get_controller_id_list
 *        tw_get_number_of_enclosures_on_controller
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_enclosure_id_list_on_controller  (
      tw_controller_id_type        controller_id,
      tw_enclosure_id_list_type*   enclosure_id_list
                                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *         Provides a method for sending SCSI/SES commands to enclosure.
 *      The API verifies that the enclosure ID is valid, then sends the request
 *      to the specified enclosure.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      scsi_cdb            - The address tw_send_enclosure_command will use for
 *                              the SCSI CDB.
 *                              Must be one of these commands:
 *                                TW_SCSI_INQUIRY
 *                                TW_SCSI_TEST_UNIT_READY
 *                                TW_SCSI_REQUEST_SENSE
 *                                TW_SCSI_RECEIVE_DIAGNOSTIC_RESULTS
 *                                TW_SCSI_SEND_DIAGNOSTIC
 *                                TW_SCSI_READ_BUFFER
 *                                TW_SCSI_WRITE_BUFFER
 *
 *      cdb_size            - The size of the SCSI CDB.
 *      status              - The command completion Status.
 *      sense_buffer        - The address tw_send_enclosure_command will use for
 *                              storing the sense data in response to SCSI command sent.
 *                              The sense buffer must be of the size as required by the standard.
 *      sense_buffer_size   - The size of the sense data buffer.
 *      payload             - The address tw_send_enclosure_command will use for
 *                              data, if any, associated with the SCSI command.
 *      payload_size        - The payload size.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified SCSI command has executed successfully.
 *      TW_RET_FAILED       - The specified SCSI command has NOT executed.
 *
 *    Remarks:
 *      Use this routine to issue a SCSI/SES command, as defined in the
 *      SCSI/SES specification, to a particular enclosure. A limited number of commands
 *      are allowed to send mainly the inquiry, enclosure management commands etc.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      SCSI/SES specifications can be found at http://www.t10.org
 * </pre>
 *
 */
int tw_send_enclosure_command(
      tw_enclosure_id_type    enclosure_id,
      unsigned char*          scsi_cdb,
      int                     cdb_size,
      unsigned char*          status,
      unsigned char*          sense_buffer,
      int                     sense_buffer_size,
      unsigned char*          payload,
      int                     payload_size
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure logical identifier of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure logical identifier of the specified enclosure.
 *      The enclosure logical identifier is written to the location specified
 *      by the logical_identifier parameter.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      logical_identifier  - The address where tw_get_enclosure_logical_identifier
 *                              will store the enclosure logical identifier.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure logical identifier for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The enclosure logical identifier for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Client application can use this routine to get an enclosure logical identifier
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_logical_identifier (
      tw_enclosure_id_type                    enclosure_id,
      tw_enclosure_logical_identifier_type*   logical_identifier
                                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure serial number of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure serial number of the specified enclosure.
 *      The enclosure serial number is written to the location specified
 *      by the serial_number parameter.
 *
 *    Parameters:
 *      enclosure_id     - ID of the enclosure in question.
 *      serial_number    - The address where tw_get_enclosure_serial_number
 *                           will store the enclosure serial number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The enclosure serial number for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The enclosure serial number for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Client application can use this routine to get an enclosure serial number
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_serial_number (
      tw_enclosure_id_type               enclosure_id,
      tw_enclosure_serial_number_type*   serial_number
 
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure vendor name of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure vendor name of the specified enclosure. The enclosure vendor name
 *      is written to the location specified by the vendor_name parameter.
 *
 *    Parameters:
 *      enclosure_id    - ID of the enclosure in question.
 *      vendor_name     - The address where tw_get_enclosure_vendor_name
 *                          will store the enclosure vendor name.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The enclosure vendor name for the specified enclosure
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The enclosure vendor name for the specified enclosure
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *      Client application can use this routine to get an enclosure vendor name
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_vendor_name (
      tw_enclosure_id_type        enclosure_id,
      tw_enclosure_vendor_type*   vendor_name
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure product name of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure product name of the specified enclosure. The enclosure product name
 *      is written to the location specified by the product_name parameter.
 *
 *    Parameters:
 *      enclosure_id    - ID of the enclosure in question.
 *      product_name    - The address where tw_get_enclosure_product_name
 *                          will store the enclosure product name.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The enclosure product name for the specified enclosure
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The enclosure product name for the specified enclosure
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *      Application can use this routine to get an enclosure product name
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_product_name (
      tw_enclosure_id_type        enclosure_id,
      tw_enclosure_product_type*  product_name
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure product revision of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure product revision of the specified enclosure. The enclosure
 *      product revision is written to the location specified by the
 *      product_revision parameter.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      product_revision    - The address where tw_get_enclosure_product_revision
 *                              will store the enclosure product revision.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure product revision for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The enclosure product revision for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Application can use this routine to get an enclosure product revision
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_product_revision (
      tw_enclosure_id_type                  enclosure_id,
      tw_enclosure_product_revision_type*   product_revision
                            );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9750 and higher)
 *
 *    Description:
 *      Provides the enclosure firmware version of the specified enclosure.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      fw_version          - The address where tw_get_enclosure_fw_version
 *                              will store the enclosure firmware version information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure fw version for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The enclosure fw version for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Application can use this routine to get an enclosure firmware version
 *      by a enclosure ID.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_fw_version (
      tw_enclosure_id_type             enclosure_id,
      tw_enclosure_fw_version_type    *fw_version
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the string of characters that describes the present state
 *      of the enclosure and provides help text describing what corrective actions
 *      should be performed on the enclosure to change it from its present state to
 *      a fully operational state.
 *      The API verifies that the enclosure ID is valid, then requests help text
 *      of the specified enclosure and the string value is written to the
 *      location specified by the help_text parameter.
 *
 *    Parameters:
 *      enclosure_id      - ID of the enclosure in question.
 *      help_text         - The address where tw_get_enclosure_help_text
 *                            will store the string value.
 *      help_text_len     - The length of the help text buffer.
 *        
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure help text for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The enclosure help text for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      The help text is intended to allow the writing of enclosure independent 
 *      application clients that manage enclosures to display enclosure specific text
 *      describing the state of the enclosure and explaining enclosure dependent corrective
 *      actions that may be required. The implementation of this function is not mandatory
 *      according to the specification and the API will return error if not supported.
 *
 *      Client application can use this routine to get an enclosure help text
 *      by enclosure ID. If the length of the supplied buffer help_text_len is smaller,
 *      then this API returns buffer too small error and returns the required buffer size
 *      in help_text_len parameter. If successful returns the text in help_text and
 *      the size of the text in help_text_len parameter.
 *        
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_help_text (
      tw_enclosure_id_type    enclosure_id,
      char*                   help_text,
      int*                    help_text_len
                               );
 

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure identify state from the enclosure.
 *
 *    Parameters:
 *      enclosure_id       - ID of the enclosure of interest.
 *      on_off              - The address where
 *                              tw_get_enclosure_identify_status will
 *                              store the enclosure identify state.
 *                              The enclosure identify state will be one of:
 *                                TW_ON  - enclosure is identified.
 *                                TW_OFF - enclosure is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified enclosure
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified enclosure
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular enclosure.
 *
 *    See also:
 *      tw_identify_enclosure
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_identify_status (
      tw_enclosure_id_type       enclosure_id,
      tw_identify_status_type*   on_off
                               );


/**
 * <pre>
 *    Supported enclosures:
 *                             9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified enclosure.  The API verifies that
 *      the enclosure ID is valid, then requests the status of the specified enclosure.
 *      The status of the enclosure is written to the location specified by
 *      the enclosure_status parameter.
 *
 *    Parameters:
 *      enclosure_id      - ID of the enclosure in question.
 *      enclosure_status  - The address where tw_get_enclosure_status will store
 *                           the enclosure status,
 *                           with any of the following values:
 *                            TW_ENCL_STATUS_OK
 *                             This indicates that the fan is installed and operational.
 *                             No error conditions are known
 *                            TW_ENCL_STATUS_WARNING
 *                             This indicates that the a warning condition was detected
 *                             by the enclosure.
 *                            TW_ENCL_STATUS_FAILURE
 *                             This indicates that the a failed condition was detected
 *                             by the enclosure.
 *                            TW_ENCL_STATUS_NOT_INSTALLED
 *                             This indicates that the fan is not installed.
 *                            TW_ENCL_STATUS_UNKNOWN
 *                             The status is not known or the status not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each enclosure has an associated status that reflects the current state
 *      of the enclosure.  Applications can use this routine to obtain the current
 *      state of a particular enclosure. Refer to individual enclosure element statuses for details.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_enclosure_status (
      tw_enclosure_id_type        enclosure_id,
      tw_enclosure_status_type*   enclosure_status
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of fans in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of fans from the specified enclosure.  The number of
 *      fans in the specified enclosure is written to the location
 *      specified by the number_of_fans parameter.
 *
 *    Parameters:
 *      enclosure_id          - ID of the enclosure in question.
 *      number_of_fans        - The address where tw_get_number_of_fans
 *                                will store the number of fans in the
 *                                specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of fans for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of fans for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Storage enclosures may have cooling fans which can be monitored.
 *      This routine provides the number of fans in a particular enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_fans (
      tw_enclosure_id_type   enclosure_id,
      int*                   number_of_fans
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of fans in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all fans from the specified enclosure.
 *        The fan IDs are written to the location specified by the
 *        fan_id_list parameter.  The number of fans on the specified
 *        enclosure is written to the length element of the fan id list.
 *
 *      Parameters:
 *        enclosure_id      - ID of the enclosure in question.
 *        fan_id_list       - The address where tw_get_fan_id_list
 *                              will store the fan id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The fan id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The fan id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set fan related information must provide the ID of the
 *        fan of interest.  This routine will provide a list of all
 *        fans, by ID, in a particular enclosure.  The fan ID list
 *        is stored in a struct of type tw_fan_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> fan IDs.
 *             length                 - The number of fan IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_fan_id_list (
      tw_enclosure_id_type   enclosure_id,
      tw_fan_id_list_type*   fan_id_list
                       );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified list of fans.  The API verifies that
 *      each fan ID in fan_id_list is valid, then requests the status of the specified
 *      fans from the enclosure.  The status of fans in the fan_id_list is written to the location
 *      specified by the status_list parameter.
 *
 *    Parameters:
 *      fan_id_list   - ID list of the fans in question.
 *      status_list   - The address where tw_get_status_of_fans will store
 *                        the status of fans specified in fan_id_list,
 *                        with any of the following values:
 *                          TW_ENCL_FAN_STATUS_OK
 *                            This indicates that the fan is installed and operational.
 *                            No error conditions are known
 *                          TW_ENCL_FAN_STATUS_FAILURE
 *                            This indicates that the fan is malfunctioning or
 *                            fan failure occurred.
 *                          TW_ENCL_FAN_STATUS_NOT_INSTALLED
 *                            This indicates that the fan is not installed.
 *                          TW_ENCL_FAN_STATUS_OFF
 *                            This indicates the fan is not providing cooling due to one of
 *                            the following reasons,
 *                              - fan has been manually turned off.
 *                              - requested by management application to turn off.
 *                              - A failure caused fan stop operating.
 *                          TW_ENCL_FAN_STATUS_UNKNOWN
 *                            The status is unknown or is not reportable.
 *      status_list_size  - size of the status_list parameter(in bytes).
 *                           This should be at least of the size,
 *                             { sizeof(tw_fan_status_type) * fan_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status of the specified fans
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status of the specified fans
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each fan has an associated status that reflects the current state
 *      of the fan.  Applications can use this routine to obtain the current
 *      state of a list of fans. All fans in the fan_id_list must belong to a
 *      same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_fan_status
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_status_of_fans(
      tw_fan_id_list_type  fan_id_list,
      tw_fan_status_type*  status_list,
      int                  status_list_size
     );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the speed of the specified list of fans.  The API verifies that
 *      each fan ID in fan_id_list is valid, then sends request to get the speed of
 *      specified fans from the enclosure.  The speed values of fans in the fan_id_list
 *      is written to the location specified by the speed_list & rpm_list parameter.
 *
 *    Parameters:
 *      fan_id_list   - ID list of the fans in question.
 *      speed_list   - The address where tw_get_speed_of_fans will store
 *                        the speed code of the fans specified in fan_id_list,
 *                        with any of the following values:
 *                         TW_ENCL_FAN_STOPPED
 *                           This indicates that the fan is stopped.
 *                         TW_ENCL_FAN_SPEED_LOWEST
 *                           This indicates that the fan is running at lowest speed.
 *                         TW_ENCL_FAN_SPEED_SECOND_LOWEST
 *                           This indicates that the fan is running at second lowest speed.
 *                         TW_ENCL_FAN_SPEED_THIRD_LOWEST
 *                           This indicates that the fan is running at third lowest speed.
 *                         TW_ENCL_FAN_SPEED_INTERMEDIATE
 *                           This indicates that the fan is running at intermediate speed.
 *                         TW_ENCL_FAN_SPEED_THIRD_HIGHEST
 *                           This indicates that the fan is running at third highest speed.
 *                         TW_ENCL_FAN_SPEED_SECOND_HIGHEST
 *                           This indicates that the fan is running at second highest speed.
 *                         TW_ENCL_FAN_SPEED_HIGHEST
 *                           This indicates that the fan is running at highest speed.
 *      speed_list_size  - size of the speed_list parameter(in bytes).
 *                           This should be at least of the size,
 *                             { sizeof(tw_fan_speed_type) * fan_id_list.length }
 *      rpm_list      - The address where tw_get_speed_of_fans will store
 *                        the speed in RPM of the fans specified in fan_id_list,
 *      rpm_list_size  - size of the rpm_list parameter(in bytes).
 *                           This should be at least of the size,
 *                             { sizeof(int) * fan_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The fan speed of the specified fans
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The fan speed of the specified fans
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current fan speed settings
 *      and the current actual speed revolutions per minute (RPM) of a list of fans.
 *      All fans in the fan_id_list must belong to the same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_fan_speed
 *      tw_get_fan_speed_in_rpm
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_speed_of_fans(
      tw_fan_id_list_type  fan_id_list,
      tw_fan_speed_type*   speed_list,
      int                  speed_list_size,
      int*                 rpm_list,
      int                  rpm_list_size
     );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the identify state of the specified list of fans.  The API verifies that
 *      each fan ID in fan_id_list is valid, then requests the identify state of the specified
 *      fan from the enclosure.  The identify state of fans in the fan_id_list is written
 *      to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      fan_id_list   - ID list of the fans in question.
 *      status_list   - The address where tw_get_identify_status_of_fans will store
 *                        the identify state of fans specified in fan_id_list,
 *                        and will be one of the following values:
 *                          TW_ON  - fan is identified.
 *                          TW_OFF - fan is NOT identified.
 *      status_list_size  - size of the status_list parameter(in bytes).
 *                           This should be at least of the size,
 *                             { sizeof(tw_identify_status_type) * fan_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The identify state of the specified fans
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The identify state of the specified fans
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current identify LED
 *      state of a list of fans. All fans in the fan_id_list must belong to a
 *      same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_fan_identify_status
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_identify_status_of_fans(
      tw_fan_id_list_type       fan_id_list,
      tw_identify_status_type*  status_list,
      int                       status_list_size
     );

/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified fan.  The API verifies that
 *      the fan ID is valid, then requests the status of the specified fan
 *      from the fan's enclosure.  The status of the fan is written to the
 *      location specified by the fan_status parameter.
 *
 *    Parameters:
 *      fan_id        - ID of the fan in question.
 *      fan_status    - The address where tw_get_fan_status will store
 *                        the fan status,
 *                        with any of the following values:
 *                          TW_ENCL_FAN_STATUS_OK
 *                            This indicates that the fan is installed and operational.
 *                            No error conditions are known
 *                          TW_ENCL_FAN_STATUS_FAILURE
 *                            This indicates that the fan is malfunctioning or
 *                            fan failure occurred.
 *                          TW_ENCL_FAN_STATUS_NOT_INSTALLED
 *                            This indicates that the fan is not installed.
 *                          TW_ENCL_FAN_STATUS_OFF
 *                            This indicates the fan is not providing cooling due to one of
 *                            the following reasons,
 *                              - fan has been manually turned off.
 *                              - requested by management application to turn off.
 *                              - A failure caused fan stop operating.
 *                          TW_ENCL_FAN_STATUS_UNKNOWN
 *                            The status is unknown or is not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified fan
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified fan
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each fan has an associated status that reflects the current state
 *      of the fan.  Applications can use this routine to obtain the current
 *      state of a particular fan.
 *
 *    See also:
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_fan_status (
      tw_fan_id_type        fan_id,
      tw_fan_status_type*   fan_status
                      );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the actual speed or rate of cooling of the specified fan.
 *      The API verifies that the fan ID is valid, then requests the speed of
 *      the specified fan from the fan's enclosure.  The speed of the fan is 
 *      written to the location specified by the fan_speed parameter.
 *
 *    Parameters:
 *      fan_id       - ID of the fan in question.
 *      fan_speed    - The address where tw_get_fan_speed will store
 *                       the fan speed,
 *                       with any of the following values:
 *                         TW_ENCL_FAN_STOPPED
 *                           This indicates that the fan is stopped.
 *                         TW_ENCL_FAN_SPEED_LOWEST
 *                           This indicates that the fan is running at lowest speed.
 *                         TW_ENCL_FAN_SPEED_SECOND_LOWEST
 *                           This indicates that the fan is running at second lowest speed.
 *                         TW_ENCL_FAN_SPEED_THIRD_LOWEST
 *                           This indicates that the fan is running at third lowest speed.
 *                         TW_ENCL_FAN_SPEED_INTERMEDIATE
 *                           This indicates that the fan is running at intermediate speed.
 *                         TW_ENCL_FAN_SPEED_THIRD_HIGHEST
 *                           This indicates that the fan is running at third highest speed.
 *                         TW_ENCL_FAN_SPEED_SECOND_HIGHEST
 *                           This indicates that the fan is running at second highest speed.
 *                         TW_ENCL_FAN_SPEED_HIGHEST
 *                           This indicates that the fan is running at highest speed.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The speed for the specified fan
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The speed for the specified fan
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current actual speed
 *      of a particular fan.
 *
 *    See also:
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_fan_speed (
      tw_fan_id_type        fan_id,
      tw_fan_speed_type*   fan_speed
                      );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the actual speed in revolutions per minute (RPM) of the specified fan.
 *      The API verifies that the fan ID is valid, then requests the speed of
 *      the specified fan from the fan's enclosure.  The speed of the fan is 
 *      written to the location specified by the fan_rpm parameter.
 *
 *    Parameters:
 *      fan_id          - ID of the fan in question.
 *      fan_rpm         - The address where tw_get_fan_speed_in_rpm will store
 *                          the fan speed,
 *                          
 *    Return Values:
 *      TW_RET_SUCCESS    - The speed for the specified fan
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The speed for the specified fan
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the actual speed of a particular fan
 *      in revolutions per minute (RPM).
 *
 *    See also:
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_fan_speed_in_rpm (
      tw_fan_id_type        fan_id,
      int*                  fan_rpm
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified fan.
 *      The API verifies that the fan ID is valid, then requests the
 *      fan identify state from the fan's enclosure.
 *
 *    Parameters:
 *      fan_id       - ID of the fan of interest.
 *      on_off              - The address where
 *                              tw_get_fan_identify_status will
 *                              store the fan identify state.
 *                              The fan identify state will be one of:
 *                                TW_ON  - fan is identified.
 *                                TW_OFF - fan is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified fan
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified fan
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular fan.
 *
 *    See also:
 *      tw_identify_fan
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fan_identify_status (
      tw_fan_id_type             fan_id,
      tw_identify_status_type*   on_off
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of drive slots in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of drive slots from the specified enclosure.  The number of
 *      drive slots in the specified enclosure is written to the location
 *      specified by the number_of_drive_slots parameter.
 *
 *    Parameters:
 *      enclosure_id           - ID of the enclosure in question.
 *      number_of_drive_slots  - The address where tw_get_number_of_drive_slots
 *                                 will store the number of drive slots in the
 *                                 specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of drive slots for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of drive slots for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Storage enclosures have drive slots which can be monitored.
 *      This routine provides the number of drive slots in a particular
 *      enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_drive_slots (
      tw_enclosure_id_type    enclosure_id,
      int*                    number_of_drive_slots
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of drive slots in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all drive slots from the specified enclosure.
 *        The drive slot IDs are written to the location specified by the
 *        drive_slot_id_list parameter.  The number of drive slots in the
 *        specified enclosure is written to the length element of the
 *        drive slot id list.
 *
 *      Parameters:
 *        enclosure_id       - ID of the enclosure in question.
 *        drive_slot_id_list - The address where tw_get_drive_slot_id_list
 *                               will store the drive slot id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The drive slot id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The drive slot id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set drive slot related information must provide the ID of the
 *        drive slot of interest.  This routine will provide a list of all
 *        drive slots, by ID, in a particular enclosure.  The drive slot ID
 *        list is stored in a struct of type tw_drive_slot_id_list_type.
 *        The following fields within the list are of interest:
 *             list                 - An array of <length> drive slot IDs.
 *             length               - The number of drive slot IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_drive_slot_id_list (
      tw_enclosure_id_type          enclosure_id,
      tw_drive_slot_id_list_type*   drive_slot_id_list
                              );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides the status of the specified list of drive slots. The API verifies
 *        that each drive slot ID in drive_slot_id_list is valid, then requests for the status
 *        of specified drive slots.  The status of drive slots in the drive_slot_id_list is written
 *        to the location specified by the status_list parameter.
 *
 *      Parameters:
 *        drive_slot_id_list - ID list of drive slots in question.
 *        status_list        - The address where tw_get_status_of_drive_slots will store
 *                              the status of drive slots specified in drive_slot_id_list.
 *                              The status value is a bitwise OR'ed combination of the following:
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_INSERTED
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_INSERT
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_REMOVE
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_ACTIVATED
 *        status_list_size  - size of the status_list parameter(in bytes).
 *                           This should be at least of the size,
 *                             { sizeof(tw_drive_slot_status_type) * drive_slot_id_list.length }
 *                                
 *      Return Values:
 *        TW_RET_SUCCESS    - The status for the specified drive slots
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The status for the specified drive slots
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive slot has an associated status that reflects the current
 *        state of the drive slot.  Applications can use this routine to obtain
 *        the current state of a list of drive slots. All drive slots in the 
 *        drive_slot_id_list must belong to the same enclosure.
 *
 *        NOTE: This API is supported only with a controller setting set to
 *              supporting SES-2 protocol.
 *
 *      See also:
 *        tw_get_drive_slot_status
 *        tw_get_drive_slot_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_status_of_drive_slots (
      tw_drive_slot_id_list_type   drive_slot_id_list,
      tw_drive_slot_status_type*   status_list,
      int                          status_list_size
                                 );

/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the identify state of the specified list of drive slots.
 *      The API verifies that each drive slot ID in drive_slot_id_list is valid,
 *      then requests the identify state of the specified drive slots from the
 *      enclosure.  The identify state of drive slots in the drive_slot_id_list
 *      is written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      drive_slot_id_list - ID list of the drive slots in question.
 *      status_list        - The address where tw_get_identify_status_of_drive_slots
 *                             will store the identify state of drive slots specified
 *                             in drive_slot_id_list, will be one of the following values:
 *                               TW_ON  - drive slot is identified.
 *                               TW_OFF - drive slot is NOT identified.
 *      status_list_size   - size of the status_list parameter(in bytes).
 *                             This should be at least of the size,
 *                             { sizeof(tw_identify_status_type) * drive_slot_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The identify state of the specified drive slots
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The identify state of the specified drive slots
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current identify LED
 *      state of a list of drive slots. All drive slots in the drive_slot_id_list
 *      must belong to the same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_drive_slot_identify_status
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_identify_status_of_drive_slots(
      tw_drive_slot_id_list_type    drive_slot_id_list,
      tw_identify_status_type*      status_list,
      int                           status_list_size
     );



/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides the status of the specified drive slot.  The API verifies
 *        that the drive slot ID is valid, then requests the status of the
 *        specified drive slot from the drive slot's enclosure.  The status of
 *        the drive slot is written to the location specified by the
 *        drive_slot_status parameter.
 *
 *      Parameters:
 *        drive_slot_id     - ID of the drive slot in question.
 *        drive_slot_status - The address where tw_get_drive_slot_status will
 *                              store the drive slot status. The status value is
 *                              a bitwise OR'ed combination of the following:
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_INSERTED
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_INSERT
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_REMOVE
 *                                TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_ACTIVATED
 *                                
 *      Return Values:
 *        TW_RET_SUCCESS    - The status for the specified drive slot
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The status for the specified drive slot
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive slot has an associated status that reflects the current
 *        state of the drive slot.  Applications can use this routine to obtain
 *        the current state of a particular drive slot.
 *
 *      See also:
 *        tw_get_drive_slot_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_drive_slot_status (
      tw_drive_slot_id_type        drive_slot_id,
      tw_drive_slot_status_type*   drive_slot_status
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified drive slot.
 *      The API verifies that the drive slot ID is valid, then requests the
 *      drive slot identify state from the drive slot's enclosure.
 *
 *    Parameters:
 *      drive_slot_id       - ID of the drive slot of interest.
 *      on_off              - The address where
 *                              tw_get_drive_slot_identify_status will
 *                              store the drive slot identify state.
 *                              The drive slot identify state will be one of:
 *                                TW_ON  - drive slot is identified.
 *                                TW_OFF - drive slot is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified drive slot
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified drive slot
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular drive slot.
 *
 *    See also:
 *      tw_get_port_identify_status
 *      tw_identify_port
 *      tw_get_unit_identify_status
 *      tw_identify_unit
 *      tw_get_drive_slot_status
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_slot_identify_status (
      tw_drive_slot_id_type      drive_slot_id,
      tw_identify_status_type*   on_off
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of temperature sensors in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of temperature sensors from the specified enclosure.  The number of
 *      temperature sensors in the specified enclosure is written to the location
 *      specified by the number_of_temp_sensors parameter.
 *
 *    Parameters:
 *      enclosure_id          - ID of the enclosure in question.
 *      number_of_temp_sensors  - The address where tw_get_number_of_temp_sensors
 *                                will store the number of temperature sensors in the
 *                                specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of temperature sensors for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of temperature sensors for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Storage enclosures may have temperature sensors which allow client application to read
 *      and display or take corrective actions if necessary. This routine provides the 
 *      number of temperature sensors in a particular enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_temp_sensors (
      tw_enclosure_id_type   enclosure_id,
      int*                   number_of_temp_sensors
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of temperature sensors in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all temperature sensors from the specified enclosure.
 *        The temperature sensor IDs are written to the location specified by the
 *        temp_sensor_id_list parameter. The number of temperature sensors on the specified
 *        enclosure is written to the length element of the id list.
 *
 *      Parameters:
 *        enclosure_id          - ID of the enclosure in question.
 *        temp_sensor_id_list   - The address where tw_get_temp_sensor_id_list
 *                                  will store the temperature sensor id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The temperature sensor id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The temperature sensor id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        the temperature must provide the ID of the sensor of interest.
 *        This routine will provide a list of all temperature sensors, 
 *        by ID, in a particular enclosure.
 *        The ID list is stored in a struct of type tw_temp_sensor_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> temperature sensor IDs.
 *             length                 - The number of IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_temp_sensor_id_list (
      tw_enclosure_id_type   enclosure_id,
      tw_temp_sensor_id_list_type*   temp_sensor_id_list
                       );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified list of temperature sensors. The API verifies
 *      that each temperature sensor ID in temp_sensor_id_list is valid, then requests for the status
 *      of specified temperature sensors from the enclosure. The status of temperature sensors
 *      in the temp_sensor_id_list is written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      temp_sensor_id_list - ID list of temperature sensors in question.
 *      status_list         - The address where tw_get_status_of_temp_sensors will store
 *                             the status of temperature sensors specified in temp_sensor_id_list.
 *                             with any of the following values:
 *                               TW_ENCL_TEMP_SENSOR_STATUS_OK
 *                                 This indicates that the temperature sensor in installed
 *                                 and operational.
 *                               TW_ENCL_TEMP_SENSOR_STATUS_NOT_INSTALLED
 *                                 This indicates that the temperature sensor is not installed.
 *                               TW_ENCL_TEMP_SENSOR_STATUS_CRITICAL
 *                                 This indicates that the critical condition is detected.
 *                               TW_ENCL_TEMP_SENSOR_STATUS_UNKNOWN
 *                                 The status is unknown or is not reportable.
 *      status_list_size    - size of the status_list parameter(in bytes).
 *                             This should be at least of the size,
 *                              { sizeof(tw_temp_sensor_status_type) * temp_sensor_id_list.length }
 *      threshold_list      - The address where tw_get_status_of_temp_sensors will store
 *                             the threshold status and is bitwise OR of:
 *                               TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_FAILURE
 *                                 This indicates that the enclosure detected a temperature
 *                                 higher than a safe operating temperature or
 *                                 higher than the preset high critical threshold value.
 *                               TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_WARNING
 *                                 This indicates that the enclosure detected a temperature
 *                                 higher than a normal operating temperature or
 *                                 higher than the preset high warning threshold value.
 *                               TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_FAILURE
 *                                 This indicates that the enclosure detected a temperature
 *                                 lower than a safe operating temperature or
 *                                 lower than the preset low critical threshold value.
 *                               TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_WARNING
 *                                 This indicates that the enclosure detected a temperature
 *                                 lower than a normal operating temperature or
 *                                 lower than the preset low warning threshold value.
 *      threshold_list_size - size of the threshold_list parameter(in bytes).
 *                             This should be at least of the size,
 *                              { sizeof(tw_temp_threshold_status_type) * temp_sensor_id_list.length }
 *                                 
 *      Return Values:
 *        TW_RET_SUCCESS    - The status and threshold for the specified temperature sensors
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The status and threshold for the specified temperature sensors
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each temperature sensor has an associated status that reflects the current
 *        state of the temperature sensor.  Applications can use this routine to obtain
 *        the current state of a list of temperature sensors. All temperature sensors in the 
 *        temp_sensor_id_list must belong to the same enclosure.
 *
 *        NOTE: This API is supported only with a controller setting set to
 *              supporting SES-2 protocol.
 *
 *      See also:
 *        tw_get_temp_sensor_status
 *        tw_get_temp_threshold_status
 *        tw_get_temp_sensor_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_status_of_temp_sensors (
      tw_temp_sensor_id_list_type     temp_sensor_id_list,
      tw_temp_sensor_status_type*     status_list,
      int                             status_list_size,
      tw_temp_threshold_status_type*  threshold_list,
      int                             threshold_list_size
      );

/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the internal temperature of the enclosure at the specified
 *      list of temperature sensors. The API verifies that each temperature sensor ID
 *      in temp_sensor_id_list is valid, then sends a request to read the temperature
 *      at the specified list of temperature sensors.  The value returned
 *      is in degrees Celsius and is written to the location specified by
 *      the temperature_list parameter. 
 *
 *    Parameters:
 *      temp_sensor_id_list - ID list of the temperature sensors in question.
 *      temperature_list    - The address where tw_get_temperature_of_sensors
 *                             will store the temperature reading value of sensors
 *                             specified in temp_sensor_id_list.
 *      temp_list_size      - size of the temperature_list parameter(in bytes).
 *                             This should be at least of the size,
 *                             { sizeof(tw_temperature_type) * temp_sensor_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The temperature value of the specified temperature sensors
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The temperature value of the specified temperature sensors
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the temperature value of a list of temperature sensors.
 *      All temperature sensors in the temp_sensor_id_list must belong to the same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_temperature
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_temperature_of_sensors(
      tw_temp_sensor_id_list_type    temp_sensor_id_list,
      tw_temperature_type*           temperature_list,
      int                            list_size
     );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the identify state of the specified list of temperature sensors.
 *      The API verifies that each temperature sensor ID in temp_sensor_id_list is valid,
 *      then requests the identify state of the specified temperature sensors from the
 *      enclosure.  The identify state of temperature sensors in the temp_sensor_id_list
 *      is written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      temp_sensor_id_list - ID list of the temperature sensors in question.
 *      status_list        - The address where tw_get_identify_status_of_temp_sensors
 *                             will store the identify state of temperature sensors specified
 *                             in temp_sensor_id_list, will be one of the following values:
 *                               TW_ON  - temperature sensor is identified.
 *                               TW_OFF - temperature sensor is NOT identified.
 *      status_list_size   - size of the status_list parameter(in bytes).
 *                             This should be at least of the size,
 *                             { sizeof(tw_identify_status_type) * temp_sensor_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The identify state of the specified temperature sensors
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The identify state of the specified temperature sensors
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current identify LED
 *      state of a list of temperature sensors. All temperature sensors in the temp_sensor_id_list
 *      must belong to the same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_temp_sensor_identify_status
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_identify_status_of_temp_sensors(
      tw_temp_sensor_id_list_type    temp_sensor_id_list,
      tw_identify_status_type*      status_list,
      int                           status_list_size
     );



/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified temperature sensor.  The API verifies that
 *      the temperature sensor ID is valid, then requests the status of the specified
 *      temperature sensor from the temperature sensor's enclosure.
 *      The status of the temperature sensor is written to the location specified by
 *      the status parameter.
 *
 *    Parameters:
 *      temp_sensor_id  - ID of the temperature sensor in question.
 *      status          - The address where tw_get_temp_sensor_status will store
 *                         the temperature sensor status.
 *                         with any of the following values:
 *                          TW_ENCL_TEMP_SENSOR_STATUS_OK
 *                            This indicates that the temperature sensor in installed and operational.
 *                          TW_ENCL_TEMP_SENSOR_STATUS_NOT_INSTALLED
 *                            This indicates that the temperature sensor is not installed.
 *                          TW_ENCL_TEMP_SENSOR_STATUS_CRITICAL
 *                            This indicates that the critical condition is detected.
 *                          TW_ENCL_TEMP_SENSOR_STATUS_UNKNOWN
 *                            The status is unknown or is not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified temperature sensor
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified temperature sensor
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each temperature sensor has an associated status that reflects the current state
 *      of the temperature sensor.  Applications can use this routine to obtain the
 *      current state of a particular temperature sensor.
 *
 *    See also:
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_temp_sensor_status (
      tw_temp_sensor_id_type        temp_sensor_id,
      tw_temp_sensor_status_type*   temp_sensor_status
                        );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the internal temperature of the enclosure at the
 *      specified sensor.  The API verifies that the sensor ID is valid,
 *      then sends a request to read the temperature.  The value returned
 *      is in degrees Celsius and is written to the location specified by
 *      the temperature parameter.
 *
 *    Parameters:
 *      temp_sensor_id    - ID of the temperature sensor in question.
 *      temperature       - The address where tw_get_temperature will store
 *                            the value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - Successfully read the temperature.
 *      TW_RET_FAILED    - Failed to read the temperature. 
 *
 *    Remarks:
 *      Use this routine to read the temperature at a particular
 *      temperature sensor of the enclosure.
 *
 *    See also:
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_temperature (
      tw_temp_sensor_id_type  temp_sensor_id,
      tw_temperature_type*    temperature
                      );


/**
 * <pre>
 *    Supported enclosures:
 *                                         9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the temperature threshold status of the specified temperature sensor
 *      element.  The API verifies that the temperature sensor ID is valid, then requests
 *      the threshold status of the specified temperature sensor from the enclosure
 *      and the value is written to the location specified by threshold_status parameter.
 *
 *    Parameters:
 *      temp_sensor_id    - ID of the temperature sensor element in question.
 *      threshold_status  - The address where tw_get_temp_threshold_status will store
 *                            the value returned and is bitwise OR of:
 *                             TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_FAILURE
 *                               This indicates that the enclosure detected a temperature
 *                               higher than a safe operating temperature or
 *                               higher than the preset high critical threshold value.
 *                             TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_WARNING
 *                               This indicates that the enclosure detected a temperature
 *                               higher than a normal operating temperature or
 *                               higher than the preset high warning threshold value.
 *                             TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_FAILURE
 *                               This indicates that the enclosure detected a temperature
 *                               lower than a safe operating temperature or
 *                               lower than the preset low critical threshold value.
 *                             TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_WARNING
 *                               This indicates that the enclosure detected a temperature
 *                               lower than a normal operating temperature or
 *                               lower than the preset low warning threshold value.
 *                                    
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The temperature threshold status for the specified sensor
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The temperature threshold status for the specified sensor
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Client application can use this routine to read the over/under temperature
 *      threshold status of a particular temperature sensor element by ID.
 *
 *    See also:
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_temp_threshold_status (
      tw_temp_sensor_id_type           temp_sensor_id,
      tw_temp_threshold_status_type*   threshold_status 
                                 );
 

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified temperature sensor.
 *      The API verifies that the temperature sensor ID is valid, then requests the
 *      temperature sensor identify state from the temperature sensor's enclosure.
 *
 *    Parameters:
 *      temp_sensor_id      - ID of the temperature sensor of interest.
 *      on_off              - The address where
 *                              tw_get_temp_sensor_identify_status will
 *                              store the temperature sensor identify state.
 *                              The temperature sensor identify state will be one of:
 *                                TW_ON  - temperature sensor is identified.
 *                                TW_OFF - temperature sensor is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified temperature sensor
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified temperature sensor
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular temperature sensor.
 *
 *    See also:
 *      tw_identify_temp_sensor
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_temp_sensor_identify_status (
      tw_temp_sensor_id_type     temp_sensor_id,
      tw_identify_status_type*   on_off
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of power supply elements in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of power supply elements from the specified enclosure. The value is
 *      written to the location specified by the number_of_power_supplies parameter.
 *
 *    Parameters:
 *      enclosure_id              - ID of the enclosure in question.
 *      number_of_power_supplies  - The address where tw_get_number_of_power_supplies
 *                                  will store the number of power supply elements
                                    in the specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of power supplies for the specified
 *                           enclosure is available in the specified parameter.
 *      TW_RET_FAILED    - The number of power supplies for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Client application call this routine to get the number of power supplies
 *      in a particular enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_power_supplies (
      tw_enclosure_id_type   enclosure_id,
      int*                   number_of_power_supplies
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of power supply elements in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all power supply elements from the specified enclosure.
 *        The power supply IDs are written to the location specified by the
 *        power_supply_id_list parameter. The length field of the id list contains
 *        the number of power supply elements.
 *
 *      Parameters:
 *        enclosure_id           - ID of the enclosure in question.
 *        power_supply_id_list   - The address where tw_get_power_supply_id_list
 *                                  will store the power supply id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The power supply id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The power supply id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to read/modify
 *        the state of power supply element must provide the ID of
 *        the power supply of interest. This routine will provide a list of all
 *        power supply elements by ID, in a particular enclosure.
 *        The ID list is stored in a struct of type tw_power_supply_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> power supply IDs.
 *             length                 - The number of IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_power_supply_id_list (
      tw_enclosure_id_type            enclosure_id,
      tw_power_supply_id_list_type*   power_supply_id_list
                       );
 

/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified list of power supplies. The API verifies
 *      that each power supply ID in power_supply_id_list is valid, then requests for the status
 *      of specified power supplies from the enclosure.  The status of power supplies
 *      in the power_supply_id_list is written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      power_supply_id_list - ID list of power supplies in question.
 *      status_list          - The address where tw_get_status_of_power_supplies will store
 *                             the status of power supplies specified in power_supply_id_list.
 *                             with any of the following values:
 *                               TW_ENCL_POWER_SUPPLY_STATUS_OK
 *                                This indicates that the power supply is installed and
 *                                operational. No error conditions are known
 *                               TW_ENCL_POWER_SUPPLY_STATUS_FAILURE
 *                                This indicates that the power supply is malfunctioning or
 *                                power supply failure occurred.
 *                               TW_ENCL_POWER_SUPPLY_STATUS_NOT_INSTALLED
 *                                This indicates that the power supply is not installed.
 *                               TW_ENCL_POWER_SUPPLY_STATUS_OFF
 *                                This indicates the power supply is not providing power
 *                                due to one of the following reasons,
 *                                 - power supply has been manually turned off.
 *                                 - requested by management application to turn off.
 *                                 - A failure caused power supply to stop providing power.
 *                               TW_ENCL_POWER_SUPPLY_STATUS_UNKNOWN
 *                                The status is unknown or is not reportable.
 *      status_list_size     - size of the status_list parameter(in bytes).
 *                             This should be at least of the size,
 *                              { sizeof(tw_power_supply_status_type) * power_supply_id_list.length }
 *      threshold_list       - The address where tw_get_status_of_power_supplies will store
 *                             the threshold status and is bitwise OR of:
 *                               TW_ENCL_POWER_THRESHOLD_DC_OVER_VOLTAGE
 *                                 This indicates an over voltage condition
 *                                 has been detected at the power supply output.
 *                               TW_ENCL_POWER_THRESHOLD_DC_UNDER_VOLTAGE
 *                                 This indicates an under voltage condition
 *                                 has been detected at the power supply output.
 *                               TW_ENCL_POWER_THRESHOLD_DC_OVER_CURRENT
 *                                 This indicates an over current condition
 *                                 has been detected at the power supply output.
 *      threshold_list_size  - size of the threshold_list parameter(in bytes).
 *                             This should be at least of the size,
 *                              { sizeof(tw_power_threshold_status_type) * power_supply_id_list.length }
 *                                 
 *      Return Values:
 *        TW_RET_SUCCESS     - The status and threshold for the specified power supplies
 *                              is available in the specified location.
 *        TW_RET_FAILED      - The status and threshold for the specified power supplies
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each power supply has an associated status that reflects the current
 *        state of the power supply.  Applications can use this routine to obtain
 *        the current state of a list of power supplies. All power supplies in the 
 *        power_supply_id_list must belong to the same enclosure.
 *
 *        NOTE: This API is supported only with a controller setting set to
 *              supporting SES-2 protocol.
 *
 *      See also:
 *        tw_get_power_supply_status
 *        tw_get_power_threshold_status
 *        tw_get_power_supply_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_status_of_power_supplies (
      tw_power_supply_id_list_type     power_supply_id_list,
      tw_power_supply_status_type*     status_list,
      int                              status_list_size,
      tw_power_threshold_status_type*  threshold_list,
      int                              threshold_list_size
      );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the identify state of the specified list of power supplies.
 *      The API verifies that each power supply ID in power_supply_id_list is valid,
 *      then requests the identify state of the specified power supplies from the
 *      enclosure.  The identify state of power supplies in the power_supply_id_list
 *      is written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      power_supply_id_list - ID list of the power supplies in question.
 *      status_list        - The address where tw_get_identify_status_of_power_supplies
 *                             will store the identify state of power supplies specified
 *                             in power_supply_id_list, will be one of the following values:
 *                               TW_ON  - power supply is identified.
 *                               TW_OFF - power supply is NOT identified.
 *      status_list_size   - size of the status_list parameter(in bytes).
 *                             This should be at least of the size,
 *                             { sizeof(tw_identify_status_type) * power_supply_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The identify state of the specified power supplies
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The identify state of the specified power supplies
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the current identify LED
 *      state of a list of power supplies. All power supplies in the power_supply_id_list
 *      must belong to the same enclosure.
 *
 *      NOTE: This API is supported only with a controller setting set to
 *            supporting SES-2 protocol.
 *
 *    See also:
 *      tw_get_power_supply_identify_status
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_identify_status_of_power_supplies(
      tw_power_supply_id_list_type    power_supply_id_list,
      tw_identify_status_type*        status_list,
      int                             status_list_size
     );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified power supply.
 *      The API verifies that the power supply ID is valid, then requests
 *      the status of the specified power supply from the enclosure.
 *      The status is written to the location specified by status parameter.
 *
 *    Parameters:
 *      power_supply_id   - ID of the power supply in question.
 *      status            - The address where tw_get_power_supply_status will store
 *                           the status,
 *                           with any of the following values:
 *                            TW_ENCL_POWER_SUPPLY_STATUS_OK
 *                              This indicates that the power supply is installed and operational.
 *                              No error conditions are known
 *                            TW_ENCL_POWER_SUPPLY_STATUS_FAILURE
 *                              This indicates that the power supply is malfunctioning or
 *                              power supply failure occurred.
 *                            TW_ENCL_POWER_SUPPLY_STATUS_NOT_INSTALLED
 *                              This indicates that the power supply is not installed.
 *                            TW_ENCL_POWER_SUPPLY_STATUS_OFF
 *                              This indicates the power supply is not providing cooling due to one of
 *                              the following reasons,
 *                                - power supply has been manually turned off.
 *                                - requested by management application to turn off.
 *                                - A failure caused power supply to stop providing power.
 *                            TW_ENCL_POWER_SUPPLY_STATUS_UNKNOWN
 *                              The status is unknown or is not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified power supply
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified power supply
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each power supply has an associated status that reflects the current state
 *      of the power supply. Applications can use this routine to obtain
 *      the current state of a particular power supply.
 *
 *    See also:
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_power_supply_status (
      tw_power_supply_id_type        power_supply_id,
      tw_power_supply_status_type*   status
                      );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the power supply threshold status of the specified power supply
 *      element. The API verifies that the power supply ID is valid,
 *      then requests the threshold status of the specified power supply from
 *      the enclosure and the value is written to the location specified by
 *      the threshold_status parameter.
 *
 *    Parameters:
 *      power_supply_id     - ID of the power supply element in question.
 *      threshold_status    - The address where
 *                              tw_get_power_threshold_status will store
 *                              the value returned and is bitwise OR of:
 *                                TW_ENCL_POWER_THRESHOLD_DC_OVER_VOLTAGE
 *                                  This indicates an over voltage condition
 *                                  has been detected at the power supply output.
 *                                TW_ENCL_POWER_THRESHOLD_DC_UNDER_VOLTAGE
 *                                  This indicates an under voltage condition
 *                                  has been detected at the power supply output.
 *                                TW_ENCL_POWER_THRESHOLD_DC_OVER_CURRENT
 *                                  This indicates an over current condition
 *                                  has been detected at the power supply output.
 *                                    
 *    Return Values:
 *      TW_RET_SUCCESS    - The power supply threshold status is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The power supply threshold status is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Client application can use this routine to read the voltage/current
 *      threshold status of a particular power supply element.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_power_threshold_status (
      tw_power_supply_id_type           power_supply_id,
      tw_power_threshold_status_type*   threshold_status 
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified power supply.
 *      The API verifies that the power supply ID is valid, then requests the
 *      power supply identify state from the power supply's enclosure.
 *
 *    Parameters:
 *      power_supply_id     - ID of the power supply of interest.
 *      on_off              - The address where
 *                              tw_get_power_supply_identify_status will
 *                              store the power supply identify state.
 *                              The power supply identify state will be one of:
 *                                TW_ON  - power supply is identified.
 *                                TW_OFF - power supply is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified power supply
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified power supply
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular power supply.
 *
 *    See also:
 *      tw_identify_power_supply
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_power_supply_identify_status (
      tw_power_supply_id_type     power_supply_id,
      tw_identify_status_type*    on_off
                               );


#ifndef   Example
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of door locks in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of door locks from the specified enclosure.  The number of
 *      door locks in the specified enclosure is written to the location
 *      specified by the number_of_door_locks parameter.
 *
 *    Parameters:
 *      enclosure_id          - ID of the enclosure in question.
 *      number_of_door_locks  - The address where tw_get_number_of_door_locks
 *                                will store the number of door locks in the
 *                                specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of door locks for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of door locks for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Storage enclosures may have door locks which can be monitored.
 *      This routine provides the number of door locks in a particular
 *      enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_door_locks (
      tw_enclosure_id_type    enclosure_id,
      int*                    number_of_door_locks
                                );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of door locks in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all door locks from the specified enclosure.
 *        The door lock IDs are written to the location specified by the
 *        door_lock_id_list parameter.  The number of door locks in the
 *        specified enclosure is written to the length element of the
 *        door lock id list.
 *
 *      Parameters:
 *        enclosure_id      - ID of the enclosure in question.
 *        door_lock_id_list - The address where tw_get_door_lock_id_list
 *                              will store the door lock id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The door lock id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The door lock id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set door lock related information must provide the ID of the
 *        door lock of interest.  This routine will provide a list of all
 *        door locks, by ID, in a particular enclosure.  The door lock ID list
 *        is stored in a struct of type tw_door_lock_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> door lock IDs.
 *             length                 - The number of door lock IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_door_lock_id_list (
      tw_enclosure_id_type         enclosure_id,
      tw_door_lock_id_list_type*   door_lock_id_list
                             );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified door lock.  The API verifies
 *      that the door lock ID is valid, then requests the status of the
 *      specified door lock from the door lock's enclosure.  The status of
 *      the door lock is written to the location specified by the
 *      door_lock_status parameter.
 *
 *    Parameters:
 *      door_lock_id      - ID of the door lock in question.
 *      door_lock_status  - The address where tw_get_door_lock_status will
 *                           store the door lock status,
 *                           with any of the following values:
 *                            TW_ENCL_DOOR_LOCK_STATUS_LOCKED
 *                             This indicates that the door is currently locked.
 *                            TW_ENCL_DOOR_LOCK_STATUS_UNLOCKED
 *                             This indicates that the door is currently unlocked or
 *                             no controllable door lock installed.
 *                            TW_ENCL_DOOR_LOCK_STATUS_NOT_INSTALLED
 *                             This indicates that the door lock is not installed.
 *                            TW_ENCL_DOOR_LOCK_STATUS_UNKNOWN
 *                             The status is unknown or is not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified door lock
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified door lock
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each door lock has an associated status that reflects the current
 *      state of the door lock.  Applications can use this routine to obtain
 *      the current state of a particular door lock.
 *
 *    See also:
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_door_lock_status (
      tw_door_lock_id_type        door_lock_id,
      tw_door_lock_status_type*   door_lock_status
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                         9000 series (9650 and higher)
 *
 *    Description:
 *      Get the identify state of the specified door lock.
 *      The API verifies that the door lock ID is valid, then requests the
 *      door lock identify state from the door lock's enclosure.
 *
 *    Parameters:
 *      door_lock_id       - ID of the door lock of interest.
 *      on_off              - The address where
 *                              tw_get_door_lock_identify_status will
 *                              store the door lock identify state.
 *                              The door lock identify state will be one of:
 *                                TW_ON  - door lock is identified.
 *                                TW_OFF - door lock is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified door lock
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified door lock
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular door lock.
 *
 *    See also:
 *      tw_identify_door_lock
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_door_lock_identify_status (
      tw_door_lock_id_type       door_lock_id,
      tw_identify_status_type*   on_off
                               );

#endif /* Example */



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the number of alarms in the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      number of alarms from the specified enclosure.  The number of
 *      alarms in the specified enclosure is written to the location
 *      specified by the number_of_alarms parameter.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      number_of_alarms    - The address where tw_get_number_of_alarms
 *                              will store the number of alarms in the
 *                              specified enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The number of alarms for the specified enclosure
 *                           is available in the specified location.
 *      TW_RET_FAILED    - The number of alarms for the specified enclosure
 *                           is NOT available in the specified location.
 *
 *    Remarks:
 *      Storage enclosures may have alarms which can be monitored.
 *      This routine provides the number of alarms in a particular enclosure.
 *
 *    See also:
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_alarms (
      tw_enclosure_id_type    enclosure_id,
      int*                    number_of_alarms
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides a list of alarms in the specified enclosure.
 *        The API verifies that the enclosure ID is valid, then requests
 *        the number and IDs of all alarms from the specified enclosure.
 *        The alarm IDs are written to the location specified by the
 *        alarm_id_list parameter.  The number of alarms on the specified
 *        enclosure is written to the length element of the alarm id list.
 *
 *      Parameters:
 *        enclosure_id      - ID of the enclosure in question.
 *        alarm_id_list     - The address where tw_get_alarm_id_list
 *                              will store the alarm id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The alarm id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The alarm id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set alarm related information must provide the ID of the alarm
 *        of interest.  This routine will provide a list of all alarms, by
 *        ID, in a particular enclosure.  The alarm ID list is stored in a
 *        struct of type tw_alarm_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> alarm IDs.
 *             length                 - The number of alarm IDs in the list.
 *
 *      See also:
 *        tw_get_enclosure_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_alarm_id_list (
      tw_enclosure_id_type     enclosure_id,
      tw_alarm_id_list_type*   alarm_id_list
                         );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the status of the specified alarm.  The API verifies that
 *      the alarm ID is valid, then requests the status of the specified
 *      alarm from the alarm's enclosure.  The status of the alarm is written
 *      to the location specified by the alarm_status parameter.
 *
 *    Parameters:
 *      alarm_id       - ID of the alarm in question.
 *      alarm_status   - The address where tw_get_alarm_status will store
 *                        the alarm status.
 *                        with any of the following values:
 *                          TW_ENCL_ALARM_STATUS_OFF
 *                            This indicates that the alarm is NOT emitting a tone.
 *                          TW_ENCL_ALARM_STATUS_ON
 *                            This indicates that the alarm is emitting a tone.
 *                          TW_ENCL_ALARM_STATUS_MUTED_STATE
 *                            This indicates that the alarm is in muted state. 
 *                            No sound is emitted by the audible alarm. The mute is requested either
 *                            by the client application or through the control panel.
 *                          TW_ENCL_ALARM_STATUS_NOT_INSTALLED
 *                             This indicates that the alarm is not installed.
 *                          TW_ENCL_ALARM_STATUS_FAILURE
 *                             This indicates that the alarm is malfunctioning or
 *                             some sort of failure occurred.
 *                          TW_ENCL_ALARM_STATUS_MUTED_ENCLOSURE_ERROR_DETECTED
 *                            This indicates that the alarm is in muted state. In this state,
 *                            no sound is emitted by the audible alarm. The mute is requested either
 *                            by the client application or through the control panel.
 *                            Either a critical condition or an unrecoverable error is detected.
 *                            Here error implies that there is something wrong with the enclosure, requires attention.
 *                            The alarm element is functional. 
 *                          TW_ENCL_ALARM_STATUS_UNKNOWN
 *                             The status is unknown or is not reportable.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified alarm
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified alarm
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each alarm has an associated status that reflects the current state
 *      of the alarm.  Applications can use this routine to obtain the
 *      current state of a particular alarm.
 *
 *    See also:
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_get_alarm_status (
      tw_alarm_id_type        alarm_id,
      tw_alarm_status_type*   alarm_status
                        );



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Gets the identify state of the specified alarm.
 *      The API verifies that the alarm ID is valid, then requests the
 *      alarm identify state from the alarm's enclosure.
 *
 *    Parameters:
 *      alarm_id       - ID of the alarm of interest.
 *      on_off         - The address where
 *                              tw_get_alarm_identify_status will
 *                              store the alarm identify state.
 *                              The alarm identify state will be one of:
 *                                TW_ON  - alarm is identified.
 *                                TW_OFF - alarm is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified alarm
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified alarm
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get identify status of a particular alarm.
 *
 *    See also:
 *      tw_identify_alarm
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_alarm_identify_status (
      tw_alarm_id_type           alarm_id,
      tw_identify_status_type*   on_off
                              );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the on-off state of the specified alarm.
 *      The API verifies the alarm ID, then returns alarm on-off state in the location
 *      specified by the onoff_state parameter. 
 * 
 *    Parameters:
 *      alarm_id       - ID of the alarm in question.
 *      onoff_state    - The address where tw_get_alarm_onoff_state
 *                          will store the value and will be one of:
 *                                TW_ALARM_OFF
 *                                TW_ALARM_ON
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The on-off state of the specified alarm
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The on-off state of the specified alarm
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the on-off state of a particular alarm.
 *
 *    See also:
 *      tw_set_alarm_off
 *      tw_set_alarm_on
 *      tw_get_alarm_status
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_alarm_onoff_state (
      tw_alarm_id_type       alarm_id,
      tw_onoff_state_type*    onoff_state
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the mute control setting of the specified alarm.
 *      The API verifies the alarm ID, then returns alarm mute state in the location
 *      specified by the mute_state parameter. 
 * 
 *    Parameters:
 *      alarm_id       - ID of the alarm in question.
 *      mute_state     - The address where tw_get_alarm_mute_state
 *                          will store the value and will be one of:
 *                                TW_ALARM_NOT_MUTED
 *                                TW_ALARM_MUTED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The mute control setting of the specified alarm
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The mute control setting of the specified alarm
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the mute control setting of a particular alarm.
 *      When the alarm is in the muted state, it will not emit sound regardless of the error
 *      condition that exists. When the alarm is un-muted, it may emit sound
 *      if an error condition exist.
 *
 *    See also:
 *      tw_unmute_alarm
 *      tw_mute_alarm
 *      tw_get_alarm_status
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_alarm_mute_state (
      tw_alarm_id_type       alarm_id,
      tw_mute_state_type*    mute_state
                             );



/*******************************************************************************
 * Unit Status and Monitoring Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides a list of units from the specified controller.  The API
 *        verifies that the controller ID is valid, then requests the number
 *        and IDs of all units from the specified controller.  The unit IDs
 *        are written to the location specified by unit_id_list.  The number
 *        of units on the specified controller is written to the length
 *        element of the unit id list.
 *
 *      Parameters:
 *        controller_id     - ID of the controller in question.
 *        unit_id_list      - The address where tw_get_unit_id_list
 *                              will store the unit id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The unit id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The unit id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        Applications that make calls to other routines in this API to get
 *        or set unit related information must provide the ID of the unit
 *        of interest.  This routine will provide a list of all units, by ID,
 *        on a particular controller.  The unit ID list is stored in a struct
 *        of type tw_unit_id_list_type.  The following fields within the list
 *        are of interest:
 *             list                   - An array of <length> unit IDs.
 *             length                 - The number of unit IDs in the list.
 *
 *      See also:
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_unit_id_list (
      tw_controller_id_type   controller_id,
      tw_unit_id_list_type*   unit_id_list
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the status of the specified unit.  The API verifies that
 *        the unit ID is valid, then requests the status of the specified unit
 *        from the unit's controller.  The status of the unit is written to the
 *        location specified by the status parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        status            - The address where tw_get_unit_status will store
 *                              the unit status.  The unit status value is
 *                              a bitwise OR'ed combination of the following:
 *                                TW_UNIT_STATUS_FLAG_ONLINE
 *                                TW_UNIT_STATUS_FLAG_COMPLETE
 *                                TW_UNIT_STATUS_FLAG_DRIVES_OK      (9000 only)
 *                                TW_UNIT_STATUS_FLAG_ALIVE          (9000 only)
 *                                TW_UNIT_STATUS_UNKNOWN
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The status for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The status for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated status that reflects the current state
 *        of the unit.  Applications can use this routine to obtain the current
 *        state of a particular unit.
 *
 *      See also:
 *        tw_get_unit_mode
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_unit_status (
      tw_unit_id_type        unit_id,
      tw_unit_status_type*   status
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the mode of the specified unit.  The API verifies that
 *        the unit ID is valid, then requests the mode of the specified unit
 *        from the unit's controller.  The mode of the unit is written to the
 *        location specified by the mode parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        mode              - The address where tw_get_unit_mode will
 *                              store the unit mode.
 *                              The unit mode value is one of:
 *                                TW_UNIT_MODE_OK
 *                                TW_UNIT_MODE_VERIFYING
 *                                TW_UNIT_MODE_INITIALIZING
 *                                TW_UNIT_MODE_DEGRADED
 *                                TW_UNIT_MODE_REBUILDING
 *                                TW_UNIT_MODE_RECOVERY
 *                                TW_UNIT_MODE_MIGRATING
 *                                TW_UNIT_MODE_INOPERABLE
 *                                TW_UNIT_MODE_UNKNOWN
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The mode for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The mode for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated operational mode.  Applications can use
 *        this routine to obtain the current mode of a particular unit.
 *
 *      See also:
 *        tw_get_unit_status
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
*/
int tw_get_unit_mode (
      tw_unit_id_type      unit_id,
      tw_unit_mode_type*   mode
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Indicates whether the specified unit has been initialized.
 *      The API verifies that the unit ID is valid, then requests the
 *      initialization state of the specified unit from the unit's controller.
 *      The initialization state of the specified unit is written to the
 *      location specified by the "is_initialized" parameter.
 *      Only valid for RAID 1 and RAID 5 units.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      is_initialized      - The address where tw_unit_is_initialized
 *                              will store the initialization state.
 *                              Returns one of:
 *                                TW_FALSE
 *                                TW_TRUE
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Successfully got the initialization state.
 *      TW_RET_FAILED     - Failed to get the initialization state.
 *
 *    Remarks:
 *      Redundant units (RAID 1, RAID 5, RAID 10, RAID 50) must be initialized
 *      before I/O can be performed on the unit.  Applications can use this
 *      routine to determine if a particular unit has been initialized.
 *
 *    See also:
 *      tw_get_unit_status
 *      tw_get_unit_configuration
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_is_initialized ( 
      tw_unit_id_type              unit_id,
      tw_initialized_state_type*   is_initialized
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the percentage complete of the specified unit.
 *        The API verifies that the unit ID is valid, then requests the
 *        percentage complete of the specified unit from the unit's controller.
 *        The percentage complete of the unit is written to the location
 *        specified by the percentage_complete parameter.
 *
 *      Parameters:
 *        unit_id             - ID of the unit in question.
 *        percentage_complete - The address where
 *                                tw_get_unit_percentage_complete will
 *                                store the unit percentage complete.
 *                                A value of 0 indicates the operation has
 *                                not started.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The percentage complete for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The percentage complete for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Units that are initializing, rebuilding or verifying, maintain an
 *        indicator of how much of the operation has been performed.
 *        Applications can use this routine to read that indicator value.
 *
 *      See also:
 *        tw_get_unit_status
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_percentage_complete (
      tw_unit_id_type                unit_id,
      tw_percentage_complete_type*   percentage_complete
                                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Indicates whether the specified unit has an active background task.
 *      The API verifies that the unit ID is valid, then requests the task
 *      state of the specified unit from the unit's controller.  The task
 *      state of the specified unit is written to the location specified by
 *      the "is_active" parameter.
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      is_active         - The address where tw_unit_background_task_active
 *                            will store the active task state.
 *                            Returns one of:
 *                              TW_FALSE
 *                              TW_TRUE
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Successfully got the bgtask state.
 *      TW_RET_FAILED     - Failed to get the bgtask state.
 *
 *    Remarks:
 *      Units that are initializing, rebuilding or verifying, maintain an
 *      indicator of whether the operation is currently being performed.
 *      Applications can use this routine to read that indicator value.
 *      Rebuild and verify operations may be performed at scheduled time.
 *
 *    See also:
 *      tw_get_unit_status
 *      tw_get_rebuild_schedule_state
 *      tw_get_verify_schedule_state
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_background_task_active (
      tw_unit_id_type          unit_id,
      tw_bgtask_active_type*   is_active
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the local unit ID of the specified unit.
 *        The API verifies that the unit ID is valid, then requests the local
 *        unit ID of the specified unit from the unit's controller.  The local
 *        unit ID of the unit is written to the location specified by the
 *        local_unit_id parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        local_unit_id     - The address where tw_get_unit_local_unit_id
 *                              will store the local unit ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The local unit ID for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The local unit ID for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated id or number that is relative to a
 *        particular controller.  Applications can use this routine to
 *        obtain that unit id or number.
 *
 *      See also:
 *        tw_get_unit_id_list
 *        tw_get_unit_number
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_local_unit_id (
      tw_unit_id_type          unit_id,
      tw_local_unit_id_type*   local_unit_id
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the configuration of the specified unit.  The API verifies
 *        that the unit ID is valid, then requests the configuration of the
 *        specified unit from the unit's controller.  The configuration of
 *        the unit is written to the location specified by the configuration
 *        parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        configuration     - The address where tw_get_unit_configuration
 *                              will store the unit configuration.
 *                              The unit status value is one of:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_RAID_1
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *                                TW_UNIT_CONFIG_ADP
 *                                TW_UNIT_CONFIG_IBOD
 *                                TW_UNIT_CONFIG_CBOD
 *                                TW_UNIT_CONFIG_SPARE
 *                                TW_UNIT_CONFIG_RAID_SUBUNIT
 *                                TW_UNIT_CONFIG_JBOD
 *                                TW_UNIT_CONFIG_MIGRATOR
 *                                TW_UNIT_CONFIG_DEAD
 *                                TW_UNIT_CONFIG_UNKNOWN
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The configuration for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The configuration for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated configuration.  Applications can use
 *        this routine to read a particular unit's configuration.
 *
 *      See also:
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_configuration (
      tw_unit_id_type          unit_id,
      tw_configuration_type*   configuration
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the capacity of the specified unit.  The API verifies that
 *        the unit ID is valid, then requests the capacity of the specified
 *        unit from the unit's controller.  The capacity of the unit is written
 *        to the location specified by the capacity parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        capacity          - The address where tw_get_unit_capacity
 *                              will store the unit capacity.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The capacity for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The capacity for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated capacity.  Applications can use
 *        this routine to read a particular unit's capacity.
 *
 *      See also:
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_capacity (
      tw_unit_id_type     unit_id,
      tw_capacity_type*   capacity
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the stripe size of the specified unit.
 *      The API verifies that the unit ID is valid, then requests the stripe
 *      size of the specified unit from the unit's controller.  The stripe
 *      size of the unit is written to the location specified by the
 *      stripe_size parameter.  Stripe size is supported for the following
 *      unit configurations:
 *        TW_UNIT_CONFIG_RAID_0
 *        TW_UNIT_CONFIG_RAID_5
 *        TW_UNIT_CONFIG_RAID_6
 *        TW_UNIT_CONFIG_RAID_10
 *        TW_UNIT_CONFIG_RAID_50
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      stripe_size       - The address where tw_get_unit_stripe_size
 *                            will store the stripe size (in K bytes).
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The stripe size for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The stripe size for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      RAID 0, 5, 10 and 50 units have an associated data stripe size.
 *      Applications can use this routine to read a particular unit's
 *      stripe size.  Stripe sizes are specified when the unit is created.
 *
 *    See also:
 *      tw_create_unit
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_stripe_size (
      tw_unit_id_type   unit_id,
      int*              stripe_size
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series ( 9650 and higher)
 *    Description:
 *      Provides the read cache policy setting of the specified unit.  The API
 *      verifies that the unit ID is valid, then requests the read cache
 *      setting of the specified unit from the unit's controller.  The read
 *      cache policy setting is written to the location specified by
 *      the policy parameter.
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      policy            - The address where tw_get_unit_read_cache_policy
 *                            will store the read cache setting and
 *                            the value will be one of:
 *                              TW_UNIT_READ_CACHE_DISABLED
 *                                 - Unit read cache is disabled.
 *                              TW_UNIT_READ_CACHE_BASIC
 *                                 - Unit read cache is set to basic (No read-ahead).
 *                              TW_UNIT_READ_CACHE_INTELLIGENT
 *                                 - Unit read cache is set to intelligent (read-ahead enabled).
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The read cache policy setting for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The read cache policy setting for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Units have an associated read cache.  Applications can use this
 *      routine to read the state a particular unit's read cache setting.
 *      Read cache settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_set_unit_read_cache_policy
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_read_cache_policy (
      tw_unit_id_type             unit_id,
      tw_unit_cache_policy_type*  policy
                                 );

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the write cache state of the specified unit.  The API
 *      verifies that the unit ID is valid, then requests the write cache
 *      state of the specified unit from the unit's controller.  The write
 *      cache state of the unit is written to the location specified by
 *      the write_cache_state parameter.
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      write_cache_state - The address where tw_get_unit_write_cache_state
 *                            will store the unit write cache state.
 *                            The unit write cache value is one of:
 *                              TW_UNIT_CACHE_DISABLED
 *                              TW_UNIT_CACHE_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The write cache state for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The write cache state for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Units have an associated write cache.  Applications can use this
 *      routine to read the state a particular unit's write cache.
 *      Write cache settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_flush_unit_write_cache
 *      tw_enable_unit_write_cache
 *      tw_disable_unit_write_cache
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_write_cache_state (
      tw_unit_id_type             unit_id,
      tw_unit_cache_state_type*   write_cache_state
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the auto verify policy of the specified unit.  The API
 *      verifies that the unit ID is valid, then requests the auto verify
 *      policy of the specified unit from the unit's controller.  The auto
 *      verify policy of the unit is written to the location specified by
 *      the autoverify parameter.
 * 
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      autoverify          - The address where tw_get_unit_auto_verify will
 *                              store the unit auto verify policy.
 *                              The unit auto verify value is one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The auto verify policy for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The auto verify policy for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Units have an associated auto verify policy.  Applications can use
 *      this routine to read a particular unit's auto verify policy state.
 *      Units with the auto verify policy enabled will have a verify operation
 *      performed according to the verify task schedule.  Auto verify policy
 *      settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_enable_unit_auto_verify
 *      tw_disable_unit_auto_verify
 *      tw_get_verify_schedule_state
 *      tw_get_task_schedule
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_auto_verify_policy (
      tw_unit_id_type         unit_id,
      tw_policy_state_type*   autoverify
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the ECC error policy of the specified unit.  The API
 *      verifies that the unit ID is valid, then requests the ECC error
 *      policy of the specified unit from the unit's controller.  The
 *      ECC policy of the unit is written to the location specified by
 *      the ecc_policy parameter.
 * 
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      ecc_policy          - The address where tw_get_unit_ecc_policy will
 *                              store the ecc policy.
 *                              The unit ecc policy value is one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The ECC error policy for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The ECC error policy for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Units have an associated ecc policy.  Applications can use
 *      this routine to read a particular unit's ECC policy state.
 *      Units with the ECC policy enabled will override ECC errors 
 *      encountered on the source drive(s) during a rebuild operation.
 *      This allows a rebuild to complete by ignoring ECC errors, at the
 *      risk of some data.
 *
 *    See also:
 *      tw_unit_set_ecc_policy_to_halt
 *      tw_unit_set_ecc_policy_to_continue
 *      tw_get_rebuild_schedule_state
 *      tw_get_task_schedule
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_ecc_policy (
      tw_unit_id_type         unit_id,
      tw_policy_state_type*   ecc_policy
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the name of the specified unit.  The API verifies that
 *        the unit ID is valid, then requests the name of the specified
 *        unit from the unit's controller.  The name of the unit is written
 *        to the location specified by the name parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        name              - The address where tw_get_unit_name
 *                              will store the unit name.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The name for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The name for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated name.  Applications can use
 *        this routine to read a particular unit's name.  Unit names
 *        are operator specified, null terminated, ascii strings.
 *
 *      See also:
 *        tw_set_unit_name
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_name (
      tw_unit_id_type      unit_id,
      tw_unit_name_type*   name
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the serial number of the specified unit.  The API verifies
 *        that the unit ID is valid, then requests the serial number of the
 *        specified unit from the unit's controller.  The serial number of
 *        the unit is written to the location specified by the
 *        serial_number parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        serial_number     - The address where tw_get_unit_serial_number
 *                              will store the unit serial number.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The serial number for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The serial number for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated serial number.  Applications can
 *        use this routine to read a particular unit's serial number.
 *        Unit serial numbers are assigned when the unit is created.
 *
 *      See also:
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_serial_number (
      tw_unit_id_type               unit_id,
      tw_unit_serial_number_type*   serial_number
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and above)
 *
 *    Description:
 *      Reads the storsave mode of the specified unit.
 *      The API verifies that the unit ID is valid, then requests
 *      the storsave mode of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      storsave_mode       - The address where tw_get_unit_storsave_mode
 *                              will store the storsave mode.
 *                              Value will be one of:
 *                                TW_STORSAVE_PROTECTION
 *                                TW_STORSAVE_BALANCE
 *                                TW_STORSAVE_PERFORMANCE
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The storsave mode for the specified unit
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The storsave mode for the specified unit
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular unit's level of
 *      data protection vs performance:
 *
 *        TW_STORSAVE_PROTECTION  - unit provides maximum data protection.
 *          "Write Cache" will be disabled when the unit becomes "DEGRADED". 
 *          Data in unit cache will be flushed to media.
 *          Host commands with an FUA (Force Unit Access) flag will be ignored
 *            if a BBU is installed and enabled;  Honored otherwise.
 *
 *        TW_STORSAVE_BALANCE     - 
 *          "Write Cache" will not be disabled when the unit becomes "DEGRADED".
 *          Data in unit cache will be flushed to media if a BBU is
 *            installed and enabled; Otherwise, data will be flushed to drive.
 *            Data flushed to the drive may, possibly, be in the drive cache
 *            and not yet on the drive media.
 *          Host commands with an FUA (Force Unit Access) flag will be ignored
 *            if a BBU is installed and enabled;  Honored otherwise.
 *
 *        TW_STORSAVE_PERFORMANCE - unit provides maximum performance.
 *          "Write Cache" will not be disabled when the unit becomes "DEGRADED".
 *          Data in unit cache will be flushed to drive.  Data flushed to
 *            the drive may, possibly, be in the drive cache and not yet on the
 *            drive media.
 *          Host commands with an FUA (Force Unit Access) flag will be ignored.
 *
 *    See also:
 *      tw_set_unit_storsave_mode
 *      tw_unit_storsave_protection
 *      tw_unit_storsave_balance
 *      tw_unit_storsave_performance
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_storsave_mode (
      tw_unit_id_type     unit_id,
      tw_storsave_type*   storsave_mode
                              );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the queue mode of the specified unit.  The API
 *      verifies that the unit ID is valid, then requests the queue mode
 *      of the specified unit from the unit's controller.  The queue mode
 *      of the unit is written to the location specified by the
 *      unit_queue_mode parameter.
 * 
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      unit_queue_mode     - The address where tw_get_unit_queue_mode will
 *                              store the unit queue mode.
 *                              The unit queue mode value is one of:
 *                                TW_POLICY_DISABLED
 *                                TW_POLICY_ENABLED
 *
 *    Return Values:
 *        TW_RET_SUCCESS    - The queue mode for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The queue mode for the specified unit
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Units have an associated queue mode.  Applications can use
 *      this routine to read a particular unit's queue mode state.
 *      Units with the queue mode enabled will have I/O commands use
 *      disk DMA queuing.  Units with the queue mode disabled will
 *      NOT have I/O commands use disk DMA queuing.
 *      Queue mode settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_enable_unit_queuing
 *      tw_disable_unit_queuing
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *    Serial ATA specifications can be found at http://www.serialata.org
 * </pre>
 *
 */
int tw_get_unit_queue_mode (
          tw_unit_id_type            unit_id,
          tw_unit_queue_mode_type*   unit_queue_mode
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Gets the identify state of the specified unit.
 *      The API verifies that the unit ID is valid, then requests the
 *      identify state of the ports associated with specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit of interest.
 *      on_off              - The address where
 *                              tw_get_unit_identify_status will
 *                              store the unit identify state.
 *                              The unit identify state will be one of:
 *                                TW_ON  - unit is identified.
 *                                TW_OFF - unit is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified unit
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified unit
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to get the identify state of the ports associated with
 *      a particular unit.
 *
 *    See also:
 *      tw_get_port_identify_status
 *      tw_identify_port
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_identify_status (
      tw_unit_id_type           unit_id,
      tw_identify_status_type*   on_off
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the LUN count of the specified unit.  The API verifies that
 *        the unit ID is valid, then requests the LUN count of the specified
 *        unit from the unit's controller.  The LUN count of the unit is written
 *        to the location specified by the lun_count parameter.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        lun_count         - The address where tw_get_unit_lun_count
 *                              will store the specified unit's LUN count.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The LUN count for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The LUN count for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each unit has an associated LUN count.  Applications can use
 *        this routine to read a particular unit's LUN count.
 *
 *      See also:
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_lun_count (
      tw_unit_id_type   unit_id,
      int*              lun_count
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Provides the Rapid RAID Recovery control settings of the specified unit.
 *      The API verifies that the unit ID is valid, then requests for
 *      the Rapid RAID Recovery control settings of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      rrc_control         - The address where tw_get_unit_rapid_recovery_control
 *                              will store the value, will be one of:
 *                                TW_RAPID_RECOVERY_DISABLE
 *                                TW_RAPID_RECOVERY_REBUILD
 *                                TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The Rapid RAID Recovery control settings for the specified unit
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The Rapid RAID Recovery control settings for the specified unit
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular unit's Rapid RAID Recovery control settings:
 *
 *        TW_RAPID_RECOVERY_DISABLE  - 
 *          Fast rebuild is disabled; Fast unclean shutdown recovery is disabled.
 *          Once set to DISABLE, the settings are not changeable.
 *          In order to change the settings from DISABLE, you will have to delete and re-create the unit. 
 *
 *        TW_RAPID_RECOVERY_REBUILD  -
 *          Fast rebuild is enabled; Fast unclean shutdown recovery is disabled.
 *
 *        TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN  - 
 *          Fast rebuild is enabled; Fast unclean shutdown recovery is enabled.
 *
 *      At the time of unit creation, by default the controller will set it to
 *      TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN.
 *
 *    See also:
 *      tw_set_unit_rapid_recovery_control
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_rapid_recovery_control (
      tw_unit_id_type                   unit_id,
      tw_rapid_recovery_control_type*   rrc_control
                                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Sets the Rapid RAID Recovery control settings for the specified unit.
 *      The API verifies that the unit ID is valid, then sets the
 *      Rapid RAID Recovery control settings of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      rrc_control         - Rapid RAID Recovery control settings of the unit,
 *                              must be one of:
 *                                TW_RAPID_RECOVERY_DISABLE
 *                                TW_RAPID_RECOVERY_REBUILD
 *                                TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully set the Rapid RAID Recovery control settings
 *                              for the specified unit.
 *      TW_RET_FAILED       - Failed to set the Rapid RAID Recovery control settings
 *                              for the specified unit.
 *                              See tw_get_error_information() for error details.
 *
 *    Remarks:
 *      Use this routine to set a particular unit's Rapid Raid Recovery control settings:
 *
 *        TW_RAPID_RECOVERY_DISABLE  - 
 *          Disable fast rebuild; Disable fast unclean shutdown recovery.
 *          Once is set to DISABLE, the settings is not changeable.
 *          In order to change the settings from DISABLE, you will have to delete and re-create the unit. 
 *
 *        TW_RAPID_RECOVERY_REBUILD  -
 *          Enable fast rebuild; Disable fast unclean shutdown recovery.
 *
 *        TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN  - 
 *          Enable fast rebuild; Enable fast unclean shutdown recovery.
 *
 *      At the time of unit creation, by default the controller will set it to
 *      TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN. Applications can use this routine
 *      to modify the settings.
 *
 *    See also:
 *      tw_get_unit_rapid_recovery_control
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_unit_rapid_recovery_control (
      tw_unit_id_type                   unit_id,
      tw_rapid_recovery_control_type    rrc_control
                                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Provides the write command DPO bit (Disable Page Out) override policy settings
 *      of the specified unit. The API verifies that the unit ID is valid, then requests
 *      the write command DPO policy settings of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      dpo_policy          - The address where tw_get_unit_write_dpo_policy
 *                              will store the value, will be one of:
 *                                TW_WRITE_DPO_IGNORE
 *                                TW_WRITE_DPO_FORCE
 *                                TW_WRITE_DPO_HONOR_HOST
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The write command DPO policy settings for the specified unit
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The write command DPO policy settings for the specified unit
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular unit's write command DPO policy settings.
 *      The controller will override the DPO bit on WRITE commands based on this value:
 *
 *        TW_WRITE_DPO_IGNORE  - 
 *          Ignore DPO bit in host WRITE commands.
 *
 *        TW_WRITE_DPO_FORCE  -
 *          Always bahave as if DPO bit is set in host WRITE commands.
 *
 *        TW_WRITE_DPO_HONOR_HOST  - 
 *          Honor host DPO bit in WRITE commands.
 *
 *      At the time of unit creation, by default the controller will set it to
 *      TW_WRITE_DPO_HONOR_HOST.
 *
 *    See also:
 *      tw_set_unit_write_dpo_policy
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_write_dpo_policy (
      tw_unit_id_type             unit_id,
      tw_write_dpo_policy_type*   dpo_policy
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Sets the write command DPO bit (Disable Page Out) override policy settings
 *      of the specified unit. The API verifies that the unit ID is valid, then sets
 *      the write command DPO policy settings of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      dpo_policy          - write command DPO policy settings of the unit,
 *                              must be one of:
 *                                TW_WRITE_DPO_IGNORE
 *                                TW_WRITE_DPO_FORCE
 *                                TW_WRITE_DPO_HONOR_HOST
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully set the write command DPO policy settings
 *                              for the specified unit.
 *      TW_RET_FAILED       - Failed to set the Rapid RAID Recovery control settings
 *                              for the specified unit.
 *                              See tw_get_error_information() for error details.
 *
 *    Remarks:
 *      Use this routine to set a particular unit's write command DPO policy settings.
 *      The controller will override the DPO bit on WRITE commands based on this value:
 *
 *        TW_WRITE_DPO_IGNORE  - 
 *          Ignore DPO bit in host WRITE commands.
 *
 *        TW_WRITE_DPO_FORCE  -
 *          Always bahave as if DPO bit is set in host WRITE commands.
 *
 *        TW_WRITE_DPO_HONOR_HOST  - 
 *          Honor host DPO bit in WRITE commands.
 *
 *      At the time of unit creation, by default the controller will set it to
 *      TW_WRITE_DPO_HONOR_HOST.
 *
 *    See also:
 *      tw_get_unit_write_dpo_policy
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_unit_write_dpo_policy (
      tw_unit_id_type             unit_id,
      tw_write_dpo_policy_type    dpo_policy
                                 );




/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the number of subunits of the specified unit.  The API
 *        verifies that the unit ID is valid, then requests the number of
 *        subunits of the specified unit from the unit's controller.  The
 *        number of subunits of the unit is written to the location
 *        specified by the number_of_subunits parameter.
 *
 *      Parameters:
 *        unit_id             - ID of the unit in question.
 *        number_of_subunits  - The address where tw_get_number_of_subunits
 *                                will store the unit number of subunits.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The number of subunits for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The number of subunits for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        RAID units are made up of multiple layers of subunits.
 *        Applications can use this routine to determine the number
 *        of subunits that a particular unit has.
 *
 *      See also:
 *        tw_get_subunit_id_list
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_subunits (
      tw_unit_id_type   unit_id,
      int*              number_of_subunits
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides a list of subunits of the specified unit.  The API verifies
 *        that the unit ID is valid, then requests the number and IDs of all
 *        subunits on the specified unit from the unit's controller.  The
 *        subunit IDs are written to the location specified by subunit_id_list.
 *        The number of subunits of the specified unit is written to the length
 *        element of the subunit id list.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        subunit_id_list   - The address where tw_get_subunit_id_list
 *                              will store the subunit id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The subunit id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The subunit id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        RAID units are made up of multiple layers of subunits.
 *        Applications that make calls to other routines in this API to get
 *        or set (sub)unit related information must provide the ID of the
 *        (sub)unit of interest.  This routine will provide a list of all
 *        subunits, by ID, of a particular unit.  The subunit ID list is
 *        stored in a struct of type tw_subunit_id_list_type.  The following
 *        fields within the list are of interest:
 *             list                   - An array of <length> subunit IDs.
 *             length                 - The number of subunit IDs in the list.
 *
 *      See also:
 *        tw_get_number_of_subunits
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_subunit_id_list (
      tw_unit_id_type            unit_id,
      tw_subunit_id_list_type*   subunit_id_list
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the number of parities of the specified unit.  The API
 *        verifies that the unit ID is valid, then requests the number of
 *        parities of the specified unit from the unit's controller.  The
 *        number of parities of the unit is written to the location
 *        specified by the number_of_parities parameter.
 *
 *      Parameters:
 *        unit_id             - ID of the unit in question.
 *        number_of_parities  - The address where tw_get_number_of_parities
 *                                will store the unit's number of parities.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The number of parities for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The number of parities for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        RAID6 units are created with more than one parity.
 *        Applications can use this routine to determine the number
 *        of parities that a particular unit has.
 *
 *      See also:
 *        tw_get_parity_id_list
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_parities (
      tw_unit_id_type   unit_id,
      int*              number_of_parities
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides a list of parity ids of the specified unit.  The API verifies
 *        that the unit ID is valid, then requests the number and IDs of all
 *        parities on the specified unit from the unit's controller.  The
 *        parity IDs are written to the location specified by parity_id_list.
 *        The number of parities of the specified unit is written to the length
 *        element of the parity id list.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        parity_id_list    - The address where tw_get_parity_id_list
 *                              will store the parity id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The parity id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The parity id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        RAID6 units are created with more than one parity.
 *        Applications that make calls to other routines in this API to get
 *        mode and percentage complete related information must provide the ID of the
 *        parity of interest.  This routine will provide a list of all
 *        parities, by ID, of a particular RAID6 unit.  The parity ID list is
 *        stored in a struct of type tw_parity_id_list_type.  The following
 *        fields within the list are of interest:
 *             list                   - An array of <length> subunit IDs.
 *             length                 - The number of subunit IDs in the list.
 *
 *      See also:
 *        tw_get_number_of_parities
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_parity_id_list (
      tw_unit_id_type            unit_id,
      tw_parity_id_list_type*   parity_id_list
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of luns for a given unit ID.  The API verifies that
 *      the unit ID is valid, then requests to get total number of luns
 *      for the specified unit ID from the unit's controller and the value
 *      is written to the location specified by the lun number parameter.
 *
 *    Parameters:
 *      unit_id          - ID of the unit in question.
 *      number_of_luns   - The address where tw_get_number_of_luns
 *                           will store the return value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The total number of luns for the specified unit ID
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The total number of luns for the specified unit ID
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each unit can be carved into number of luns. Applications can use this
 *      routine to read number of luns of a particular unit by a given unit ID.
 *
 *    See also:
 *      tw_get_lun_id_list
 *      tw_get_lun_capacity
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_luns (
      tw_unit_id_type     unit_id,
      unsigned char*      number_of_luns   
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides a list of luns of the specified unit.  The API verifies
 *        that the unit ID is valid, then requests the number and IDs of all
 *        luns on the specified unit from the unit's controller.  The
 *        lun IDs are written to the location specified by lun_id_list.
 *        The number of luns of the specified unit is written to the length
 *        element of the lun id list.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        lun_id_list       - The address where tw_get_lun_id_list
 *                              will store the lun id list and length.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The lun id list and length are
 *                              available in the specified location.
 *        TW_RET_FAILED     - The lun id list is NOT valid and
 *                              the list length is set to zero.
 *
 *      Remarks:
 *        RAID units can be carved into number of luns.
 *        This routine will provide a list of all luns, by ID, of a particular unit.
 *        The lun ID list is stored in a struct of type tw_lun_id_list_type.
 *        The following fields within the list are of interest:
 *             list                   - An array of <length> lun IDs.
 *             length                 - The number of lun IDs in the list.
 *
 *      See also:
 *        tw_get_lun_capacity
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_lun_id_list (
      tw_unit_id_type        unit_id,
      tw_lun_id_list_type*   lun_id_list
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Provides the capacity of the specified lun.  The API verifies that
 *        the lun ID is valid, then requests the capacity of the specified
 *        lun from the lun's controller.  The capacity of the lun is written
 *        to the location specified by the capacity parameter.
 *
 *      Parameters:
 *        lun_id            - ID of the lun in question.
 *        capacity          - The address where tw_get_lun_capacity
 *                              will store the lun capacity.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The capacity for the specified lun
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The capacity for the specified lun
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each lun has an associated capacity. Applications can use
 *        this routine to read a particular lun's capacity. The capacity
 *        value returned is in sectors (512 bytes).
 *
 *      See also:
 *        tw_get_lun_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_lun_capacity (
      tw_lun_id_type     lun_id,
      tw_lun_size_type*  capacity
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the lun number for a given lun ID.  The API verifies that
 *      the lun ID is valid, then requests lun number of the specified
 *      lun ID from the lun's controller.  The lun number is written
 *      to the location specified by the lun number parameter.
 *
 *    Parameters:
 *      lun_id             - ID of the lun in question.
 *      lun_number         - The address where tw_get_lun_number
 *                             will store the lun number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The lun number for the specified lun ID
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The lun number for the specified lun ID
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each lun has an associated lun number assigned by firmware.  Applications
 *      can use this routine to read a particular lun's number by a given lun ID.
 *      A value of TW_NULL_LUN indicates that LUN does not exist.
 *
 *    See also:
 *      tw_get_lun_id_list
 *      tw_get_lun_capacity
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_lun_number (
      tw_lun_id_type     lun_id,
      unsigned char*     lun_number
                        );



/*******************************************************************************
 * Drive Status and Monitoring Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides a list of drives from the specified controller.  The API
 *      verifies that the controller ID is valid, then requests the number
 *      and IDs of all drives from the specified controller.  The drive IDs
 *      are written to the location specified by drive_id_list.  The number
 *      of drives on the specified controller is written to the length
 *      element of the drive id list.
 *        
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      drive_id_list     - The address where tw_get_drive_id_list
 *                            will store the drive id list.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive id list and length are
 *                            available in the specified location.
 *      TW_RET_FAILED     - The drive id list is NOT valid and
 *                            the list length is set to zero.
 *
 *    Remarks:
 *      Applications that make calls to other routines in this API to get
 *      or set drive related information must provide the ID of the drive
 *      of interest.  This routine will provide a list of all drives, by ID,
 *      on a particular controller.  The drive ID list is stored in a struct
 *      of type tw_drive_id_list_type.  The following fields within the list
 *      are of interest:
 *        list            - An array of <length> drive IDs.
 *        length          - The number of drive IDs in the list.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_unit_id_list
 *      tw_get_subunit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_id_list (
      tw_controller_id_type    controller_id,
      tw_drive_id_list_type*   drive_id_list
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the port id of the specified drive.  The API verifies that
 *      the drive ID is valid, then requests the port id of the specified
 *      drive from the drive's controller.  The port id of the drive is
 *      written to the location specified by the port_id parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      port_id           - The address where tw_get_drive_port_id will
 *                            store the drive port id.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The port id for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The port id for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each drive has an associated port ID.  Applications can use this
 *      routine to obtain that port ID.
 *
 *    See also:
 *      tw_get_drive_number
 *      tw_get_port_number
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_port_id (
      tw_drive_id_type   drive_id,
      tw_port_id_type*   port_id
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the status of the specified drive.  The API verifies that
 *      the drive ID is valid, then requests the status of the specified
 *      drive from the drive's controller.  The status of the drive is
 *      written to the location specified by the status parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      status            - The address where tw_get_drive_status will
 *                            store the drive status.
 *                            The drive status value is one of:
 *                              TW_DRIVE_STATUS_OK
 *                              TW_DRIVE_STATUS_UNSUPPORTED
 *                              TW_DRIVE_STATUS_ECC_ERROR          (9000 only)
 *                              TW_DRIVE_STATUS_POR_ERROR          (9000 only)
 *                              TW_DRIVE_STATUS_SMART_FAILURE      (9000 only)
 *                              TW_DRIVE_STATUS_DEVICE_ERROR       (9000 only)
 *                              TW_DRIVE_STATUS_DCB_READ_TIMEOUT
 *                              TW_DRIVE_STATUS_DCB_READ_FAILURE
 *                              TW_DRIVE_STATUS_CONFIG_OPERATION_FAILURE
 *                              TW_DRIVE_STATUS_DCB_ORPHAN
 *                              TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                              TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                              TW_DRIVE_STATUS_UNCONVERTED_DCB
 *                              TW_DRIVE_STATUS_REMOVED            (9000 only)
 *                              TW_DRIVE_STATUS_OFFLINE_JBOD
 *                              TW_DRIVE_STATUS_OFFLINE
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each drive has an associated status that reflects the current state
 *      of the drive.  Applications can use this routine to obtain the
 *      current state of a particular drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_status (
      tw_drive_id_type        drive_id,
      tw_drive_status_type*   status
                        );



/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the status of the specified list of drives.  The API verifies that
 *      each drive ID in drive_id_list is valid, then requests the status of the specified
 *      drives from the drive's controller.  The status of drives is
 *      written to the location specified by the status_list parameter.
 *
 *    Parameters:
 *      drive_id_list       - ID list of the drives in question.
 *      status_list         - The address where tw_get_status_of_drives will
 *                            store drive status of specified drive list.
 *                            The drive status value is one of:
 *                              TW_DRIVE_STATUS_OK
 *                              TW_DRIVE_STATUS_UNSUPPORTED
 *                              TW_DRIVE_STATUS_ECC_ERROR          (9000 only)
 *                              TW_DRIVE_STATUS_POR_ERROR          (9000 only)
 *                              TW_DRIVE_STATUS_SMART_FAILURE      (9000 only)
 *                              TW_DRIVE_STATUS_DEVICE_ERROR       (9000 only)
 *                              TW_DRIVE_STATUS_DCB_READ_TIMEOUT
 *                              TW_DRIVE_STATUS_DCB_READ_FAILURE
 *                              TW_DRIVE_STATUS_CONFIG_OPERATION_FAILURE
 *                              TW_DRIVE_STATUS_DCB_ORPHAN
 *                              TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                              TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                              TW_DRIVE_STATUS_UNCONVERTED_DCB
 *                              TW_DRIVE_STATUS_REMOVED            (9000 only)
 *                              TW_DRIVE_STATUS_OFFLINE_JBOD
 *                              TW_DRIVE_STATUS_OFFLINE
 *      status_list_size     - size of the status list parameter (in bytes).
 *                              This should be at least of the size,
 *                              { sizeof(tw_drive_status_type) * drive_id_list.length }
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The status for the specified list of drives
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The status for the specified list of drives
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Each drive has an associated status that reflects the current state
 *      of the drive.  Applications can use this routine to obtain the
 *      current state of a drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_status_of_drives (
      tw_drive_id_list_type   drive_id_list,
      tw_drive_status_type*   status_list,
      int                     status_list_size
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the capacity of the specified drive.  The API verifies that
 *        the drive ID is valid, then requests the capacity of the specified
 *        drive from the drive's controller.  The capacity of the drive is
 *        written to the location specified by the capacity parameter.
 *
 *      Parameters:
 *        drive_id          - ID of the drive in question.
 *        capacity          - The address where tw_get_drive_capacity will
 *                              store the drive capacity.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The capacity for the specified drive
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The capacity for the specified drive
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive has an associated capacity.  Applications can use
 *        this routine to read a particular drive's capacity.
 *
 *      See also:
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_capacity (
      tw_drive_id_type    drive_id,
      tw_capacity_type*   capacity
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the model of the specified drive.  The API verifies that
 *        the drive ID is valid, then requests the model of the specified
 *        drive from the drive's controller.  The model of the drive is
 *        written to the location specified by the model parameter.
 *
 *      Parameters:
 *        drive_id          - ID of the drive in question.
 *        model             - The address where tw_get_drive_model will
 *                              store the drive model.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The model for the specified drive
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The model for the specified drive
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive has an associated model.  Applications can use
 *        this routine to read a particular drive's model.
 *
 *      See also:
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_model (
      tw_drive_id_type   drive_id,
      tw_model_type*     model
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the serial number of the specified drive.  The API verifies
 *        that the drive ID is valid, then requests the serial number of the
 *        specified drive from the drive's controller.  The serial number of
 *        the drive is written to the location specified by the serial_number
 *        parameter.
 *
 *      Parameters:
 *        drive_id          - ID of the drive in question.
 *        serial_number     - The address where tw_get_drive_serial_number
 *                              will store the drive serial number.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The serial number for the specified drive
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The serial number for the specified drive
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive has an associated serial number.  Applications can use
 *        this routine to read a particular drive's serial number.
 *
 *      See also:
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_serial_number (
      tw_drive_id_type         drive_id,
      tw_serial_number_type*   serial_number
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the firmware version of the specified drive.  The API
 *        verifies that the drive ID is valid, then requests the firmware
 *        version of the specified drive from the drive's controller.
 *        The firmware version of the drive is written to the location
 *        specified by the firmware_version parameter.
 *
 *      Parameters:
 *        drive_id          - ID of the drive in question.
 *        firmware_version  - The address where tw_get_drive_firmware_version
 *                              will store the drive firmware version.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The firmware version for the specified drive
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The firmware version for the specified drive
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Each drive has an associated firmware version.  Applications can use
 *        this routine to read a particular drive's firmware version.
 *
 *      See also:
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_firmware_version (
      tw_drive_id_type            drive_id,
      tw_firmware_version_type*   firmware_version
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the SMART data of the specified drive.  The API verifies
 *      that the drive ID is valid, then requests the SMART data of the
 *      specified drive from the drive's controller.  The SMART data of
 *      the drive is written to the location specified by the smart_data
 *      parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      smart_data        - The address where tw_get_drive_smart_data
 *                            will store the drive SMART data.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The SMART data for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The SMART data for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular drive's SMART (SMART read data)
 *      data as defined in the ATA specification.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      ATA specifications can be found at http://www.t13.org
 * </pre>
 *
 */
int tw_get_drive_smart_data (
      tw_drive_id_type      drive_id,
      tw_smart_data_type*   smart_data
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the ident data of the specified drive.  The API verifies
 *      that the drive ID is valid, then requests the ident data of the
 *      specified drive from the drive's controller.  The ident data of
 *      the drive is written to the location specified by the ident_data
 *      parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      ident_data        - The address where tw_get_drive_ident_data
 *                            will store the drive ident data.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The ident data for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The ident data for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular drive's ident (identify device)
 *      data as defined in the ATA specification.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      ATA specifications can be found at http://www.t13.org
 * </pre>
 *
 */
int tw_get_drive_ident_data (
      tw_drive_id_type      drive_id,
      tw_ident_data_type*   ident_data
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the bit mask that represent whether the drive is part of an exportable unit.
 *      The API verifies that the drive ID is valid before sending the requests to the controller.
 *      The drive flags is written to the location specified by the drive_flags parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      drive_flags       - The address where tw_get_drive_flags will
 *                            store the drive flags.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive flags for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The drive flags for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to read a particular drive's flags.
 *        TW_DRIVE_FLAGS_PART_OF_EXPORTABLE_UNIT - Indicates that the drive is 
 *                                                   part of an exportable unit.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_flags (
      tw_drive_id_type    drive_id,
      tw_drive_flags_type*   drive_flags
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the queue capability of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the
 *      queue capability of the specified drive from the drive's controller.
 *      The queue capability of the drive is written to the location specified
 *      by the drive_queue_capability parameter.
 *
 *    Parameters:
 *      drive_id               - ID of the drive in question.
 *      drive_queue_capability - The address where
 *                                 tw_get_drive_queue_capability will store
 *                                 the drive queue capability.
 *                                 The drive queue capability value is one of:
 *                                   TW_POLICY_DISABLED - NCQ is NOT supported
 *                                   TW_POLICY_ENABLED - NCQ is supported
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The queue capability for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The queue capability for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Drives have an associated command queue capability.
 *      Applications can use this routine to read a particular drive's
 *      queue capability.  Drives with command queue capability can have
 *      I/O commands use disk DMA queuing.  Applications can read the drive
 *      queue mode with tw_get_drive_queue_mode.  Applications can set the
 *      drive queue mode with tw_enable_unit_queuing or
 *      tw_disable_unit_queuing.
 *      Queue capability settings are not persistent for JBOD drives.
 *
 *    See also:
 *      tw_get_unit_queue_mode
 *      tw_enable_unit_queuing
 *      tw_disable_unit_queuing
 *      tw_get_drive_queue_mode
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *    Serial ATA specifications can be found at http://www.serialata.org
 * </pre>
 *
 */
int tw_get_drive_queue_capability (
          tw_drive_id_type            drive_id,
          tw_drive_queue_mode_type*   drive_queue_capability
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the queue mode of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the
 *      queue mode of the specified drive from the drive's controller.
 *      The queue mode of the drive is written to the location specified
 *      by the drive_queue_mode parameter.
 *
 *    Parameters:
 *      drive_id               - ID of the drive in question.
 *      drive_queue_mode       - The address where
 *                                 tw_get_drive_queue_mode will store
 *                                 the drive queue mode.
 *                                 The drive queue mode value is one of:
 *                                   TW_POLICY_DISABLED - NCQ is NOT enabled
 *                                   TW_POLICY_ENABLED - NCQ is enabled
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The queue mode for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The queue mode for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to read a particular drive's
 *      queue mode.  Drives with command queue enabled have I/O commands use
 *      disk DMA queuing.  Applications can read the drive queue mode with
 *      tw_get_drive_queue_mode.  Applications can set the drive queue mode
 *      with tw_enable_unit_queuing or tw_disable_unit_queuing.
 *      Queue mode settings are not persistent for JBOD drives.
 *
 *    See also:
 *      tw_get_unit_queue_mode
 *      tw_enable_unit_queuing
 *      tw_disable_unit_queuing
 *      tw_get_drive_queue_capability
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *    Serial ATA specifications can be found at http://www.serialata.org
 * </pre>
 *
 */
int tw_get_drive_queue_mode (
          tw_drive_id_type            drive_id,
          tw_drive_queue_mode_type*   drive_queue_mode
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides a method for ATA commands.
 *
 *    Parameters:
 *      drive_id            - ID of the drive in question.
 *      flags               - The value must be a bitwise OR'ed combination
 *                              of the following flags:
 *                                TW_ATA_FLAGS_WRITE_THRU
 *                                  If this flag is set then all previous
 *                                  commands are flushed to media before
 *                                  this ATA command is issued.
 *                                TW_ATA_FLAGS_EXT_ATA_CMD
 *                                  If this flag is set then the command uses
 *                                  the extended ATA command protocol.
 *      features            - The address tw_ata_passthrough will use for
 *                              the ATA "Features/ Error" register.
 *                              When command is TW_ATA_SMART must be one of:
 *                                TW_ATA_SMART_READ_DATA
                                  TW_ATA_SMART_GET_THRESHOLD
 *                                TW_ATA_SMART_ENABLE_DISABLE_AUTOSAVE
 *                                TW_ATA_SMART_EXECUTE_OFFLINE_IMMEDIATE
 *                                TW_ATA_SMART_READ_LOG
 *                                TW_ATA_SMART_WRITE_LOG
 *                                TW_ATA_SMART_ENABLE_OPERATIONS
 *                                TW_ATA_SMART_RETURN_STATUS
 *      sector_count        - The address tw_ata_passthrough will use for
 *                              the ATA "Sector Count" register.
 *                              The size of the payload in sectors (512 bytes).
 *      lba_low             - The address tw_ata_passthrough will use for
 *                              the ATA "LBA Low" register.
 *      lba_mid             - The address tw_ata_passthrough will use for
 *                              the ATA "LBA Mid" register.
 *      lba_high            - The address tw_ata_passthrough will use for
 *                              the ATA "LBA High" register.
 *      device              - The address tw_ata_passthrough will use for
 *                              the ATA "Device" register.
 *      command             - The address tw_ata_passthrough will use for
 *                              the ATA "Command/ Status" register.
 *                              Must be one of:
 *                                TW_ATA_NOP
 *                                TW_ATA_READ_SECTOR             <= 256 sectors
 *                                TW_ATA_READ_SECTOR_EXT         <= 256 sectors
 *                                TW_ATA_READ_DMA_EXT
 *                                TW_ATA_READ_NATIVE_MAX_ADDRESS_EXT
 *                                TW_ATA_READ_STREAM_DMA
 *                                TW_ATA_READ_STREAM_PIO
 *                                TW_ATA_READ_LOG_EXT
 *                                TW_ATA_SET_MAX_ADDRESS_EXT
 *                                TW_ATA_WRITE_LOG_EXT
 *                                TW_ATA_READ_VERIFY_SECTOR
 *                                TW_ATA_READ_VERIFY_SECTOR_EXT  <= 256 sectors
 *                                TW_ATA_SEEK
 *                                TW_ATA_SMART
 *                                TW_ATA_READ_DMA
 *                                TW_ATA_IDLE_IMMEDIATE
 *                                TW_ATA_IDLE
 *                                TW_ATA_READ_BUFFER
 *                                TW_ATA_CHECK_POWER_MODE
 *                                TW_ATA_FLUSH_CACHE
 *                                TW_ATA_WRITE_BUFFER
 *                                TW_ATA_FLUSH_CACHE_EXT
 *                                TW_ATA_IDENTIFY_DEVICE
 *                                TW_ATA_SECURITY_FREEZE_LOCK
 *                                TW_ATA_READ_NATIVE_MAX_ADDRESS
 *      payload             - The address tw_ata_passthrough will use for
 *                              data, if any, associated with the ATA command.
 *                              The payload must have a size which is an
 *                              integral number of disk sectors (512 bytes)
 *                              as indicated by the sector_count value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified ATA passthrough command
 *                              has executed successfully.
 *      TW_RET_FAILED       - The specified ATA passthrough command
 *                              has NOT executed.
 *
 *    Remarks:
 *      Use this routine to issue an ATA command, as defined in the
 *      ATA specification, to a particular drive.
 *      The following commands will be supported up to a maximum of 256 sectors:
 *        TW_ATA_READ_SECTOR
 *        TW_ATA_READ_SECTOR_EXT
 *        TW_ATA_READ_VERIFY_SECTOR_EXT
 *
 *      The ATA DMA commands will be supported up to a maximum of 128 sectors
 *      on controllers 9550 and higher. The 9500s and older support up to 256 sectors:
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      ATA specifications can be found at http://www.t13.org
 * </pre>
 *
 */
int tw_ata_passthrough (
      tw_drive_id_type    drive_id,
      unsigned char       flags,
      unsigned short*     feature,
      unsigned short*     sector_count,
      unsigned short*     lba_low,
      unsigned short*     lba_mid,
      unsigned short*     lba_high,
      unsigned char*      device,
      unsigned char*      command,
      unsigned char*      payload
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides a method for sending ATA commands to drives. In addition allows
 *      to set a timeout value for each ATA command sent. The command time out value
 *      specified should be in multiples of 10 sec with a maximum value of 150 seconds.
 *      A value of 0 indicates that the controller should use the default setting.
 *      The default command timeout is 10 seconds.
 *
 *    Parameters:
 *      drive_id            - ID of the drive in question.
 *      flags               - The value must be a bitwise OR'ed combination
 *                              of the following flags:
 *                                TW_ATA_FLAGS_WRITE_THRU
 *                                  If this flag is set then all previous
 *                                  commands are flushed to media before
 *                                  this ATA command is issued.
 *                                TW_ATA_FLAGS_EXT_ATA_CMD
 *                                  If this flag is set then the command uses
 *                                  the extended ATA command protocol.
 *      features            - The address tw_ata_passthrough will use for
 *                              the ATA "Features/ Error" register.
 *                              When command is TW_ATA_SMART must be one of:
 *                                TW_ATA_SMART_READ_DATA
                                  TW_ATA_SMART_GET_THRESHOLD
 *                                TW_ATA_SMART_ENABLE_DISABLE_AUTOSAVE
 *                                TW_ATA_SMART_EXECUTE_OFFLINE_IMMEDIATE
 *                                TW_ATA_SMART_READ_LOG
 *                                TW_ATA_SMART_WRITE_LOG
 *                                TW_ATA_SMART_ENABLE_OPERATIONS
 *                                TW_ATA_SMART_RETURN_STATUS
 *      sector_count        - The address tw_ata_passthrough will use for
 *                              the ATA "Sector Count" register.
 *                              The size of the payload in sectors (512 bytes).
 *      lba_low             - The address tw_ata_passthrough will use for
 *                              the ATA "LBA Low" register.
 *      lba_mid             - The address tw_ata_passthrough will use for
 *                              the ATA "LBA Mid" register.
 *      lba_high            - The address tw_ata_passthrough will use for
 *                              the ATA "LBA High" register.
 *      device              - The address tw_ata_passthrough will use for
 *                              the ATA "Device" register.
 *      command             - The address tw_ata_passthrough will use for
 *                              the ATA "Command/ Status" register.
 *                              Must be one of:
 *                                TW_ATA_NOP
 *                                TW_ATA_READ_SECTOR             <= 256 sectors
 *                                TW_ATA_READ_SECTOR_EXT         <= 256 sectors
 *                                TW_ATA_READ_DMA_EXT
 *                                TW_ATA_READ_NATIVE_MAX_ADDRESS_EXT
 *                                TW_ATA_READ_STREAM_DMA
 *                                TW_ATA_READ_STREAM_PIO
 *                                TW_ATA_READ_LOG_EXT
 *                                TW_ATA_SET_MAX_ADDRESS_EXT
 *                                TW_ATA_WRITE_LOG_EXT
 *                                TW_ATA_READ_VERIFY_SECTOR
 *                                TW_ATA_READ_VERIFY_SECTOR_EXT  <= 256 sectors
 *                                TW_ATA_SEEK
 *                                TW_ATA_SMART
 *                                TW_ATA_READ_DMA
 *                                TW_ATA_IDLE_IMMEDIATE
 *                                TW_ATA_IDLE
 *                                TW_ATA_READ_BUFFER
 *                                TW_ATA_CHECK_POWER_MODE
 *                                TW_ATA_FLUSH_CACHE
 *                                TW_ATA_WRITE_BUFFER
 *                                TW_ATA_FLUSH_CACHE_EXT
 *                                TW_ATA_IDENTIFY_DEVICE
 *                                TW_ATA_SECURITY_FREEZE_LOCK
 *                                TW_ATA_READ_NATIVE_MAX_ADDRESS
 *      payload             - The address tw_ata_passthrough will use for
 *                              data, if any, associated with the ATA command.
 *                              The payload must have a size which is an
 *                              integral number of disk sectors (512 bytes)
 *                              as indicated by the sector_count value.
 *      timeout             - Command timeout value in seconds.
 *                              A value of 0 implies default setting(10 sec).
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified ATA passthrough command
 *                              has executed successfully.
 *      TW_RET_FAILED       - The specified ATA passthrough command
 *                              has NOT executed.
 *
 *    Remarks:
 *      Use this routine to issue an ATA command, as defined in the
 *      ATA specification, to a particular drive.
 *      The following commands will be supported up to a maximum of 256 sectors:
 *        TW_ATA_READ_SECTOR
 *        TW_ATA_READ_SECTOR_EXT
 *        TW_ATA_READ_VERIFY_SECTOR_EXT
 *
 *      The ATA DMA commands will be supported up to a maximum of 128 sectors
 *      on controllers 9550 and higher. The 9500s and older support up to 256 sectors:
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      ATA specifications can be found at http://www.t13.org
 * </pre>
 *
 */
int tw_ata_passthrough_ex (
      tw_drive_id_type    drive_id,
      unsigned char       flags,
      unsigned short*     feature,
      unsigned short*     sector_count,
      unsigned short*     lba_low,
      unsigned short*     lba_mid,
      unsigned short*     lba_high,
      unsigned char*      device,
      unsigned char*      command,
      unsigned char*      payload,
      unsigned int        timeout
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Provides a method for sending SCSI commands to drives. In addition,
 *      allows to set the timeout value for each SCSI command sent.
 *      The command time out value specified should be in multiples of 10 sec
 *      with a maximum value of 150 seconds. A value of 0 indicates that
 *      the controller should use the default setting.
 *      The default command timeout is 10 seconds.
 *
 *    Parameters:
 *      drive_id            - ID of the drive in question.
 *      flags               - The value must be a bitwise OR'ed combination
 *                              of the following flags:
 *                                TW_SCSI_FLAGS_NONE
 *      transfer_mode       - Indicates how data is to be read or written.
 *                              Must be one of:
 *                                TW_SCSI_PASSTHRU_NON_DATA
 *                                TW_SCSI_PASSTHRU_WRITE_TO_DRIVE
 *                                TW_SCSI_PASSTHRU_READ_FROM_DRIVE
 *                                TW_SCSI_PASSTHRU_BIDIRECTIONAL_DATA
 *      scsi_cdb            - The address tw_scsi_passthrough will use for
 *                              the SCSI CDB.
 *                              Must be one of these commands:
 *                                TW_SCSI_INQUIRY
 *                                TW_SCSI_LOG_SENSE
 *                                TW_SCSI_MODE_SENSE_6 
 *                                TW_SCSI_MODE_SENSE_10
 *                                TW_SCSI_READ_6
 *                                TW_SCSI_READ_10
 *                                TW_SCSI_READ_12
 *                                TW_SCSI_READ_16
 *                                TW_SCSI_READ_BUFFER 
 *                                TW_SCSI_READ_CAPACITY_10 
 *                                TW_SCSI_READ_CAPACITY_16 
 *                                TW_SCSI_READ_DEFECT_DATA_10 
 *                                TW_SCSI_READ_DEFECT_DATA_12 
 *                                TW_SCSI_READ_MEDIA_SERIAL_NUMBER
 *                                TW_SCSI_RECEIVE_DIAGNOSTIC_RESULTS
 *                                TW_SCSI_REPORT_ALIASES
 *                                TW_SCSI_REPORT_DEVICE_IDENTIFIER
 *                                TW_SCSI_REPORT_LUNS
 *                                TW_SCSI_REPORT_PRIORITY
 *                                TW_SCSI_REPORT_SUPPORTED_OPERATION_CODES 
 *                                TW_SCSI_REPORT_SUPPORTED_TASK_MANAGEMENT_FUNCTIONS
 *                                TW_SCSI_REPORT_TARGET_PORT_GROUPS
 *                                TW_SCSI_REPORT_TIMESTAMP
 *                                TW_SCSI_TEST_UNIT_READY
 *                                TW_SCSI_VERIFY_10
 *                                TW_SCSI_VERIFY_12
 *                                TW_SCSI_VERIFY_16
 *
 *      cdb_size            - The size of the SCSI CDB.
 *      status              - The drive status.
 *      sense_buffer        - The address tw_scsi_passthrough will use for
 *                              storing the drive's sense data in response to SCSI command sent.
 *                              The sense buffer should be of the size required by the standard
 *      sense_buffer_size   - The size of the sense data buffer.
 *      payload             - The address tw_ata_passthrough will use for
 *                              data, if any, associated with the SCSI command.
 *                              The payload for IO READ/WRITE commands must have a size
 *                              which is an integral number of disk sectors (512 bytes)
 *      payload_size        - The payload size.
 *      timeout             - Command timeout value in seconds.
 *                              A value of 0 implies default setting (10 sec).
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified SCSI pass-through command
 *                              has executed successfully.
 *      TW_RET_FAILED       - The specified SCSI pass-through command
 *                              has NOT executed.
 *
 *    Remarks:
 *      Use this routine to issue an SCSI command, as defined in the
 *      SCSI specification, to a particular drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      SCSI specifications can be found at http://www.t10.org
 * </pre>
 *
 */
int tw_scsi_passthrough (
      tw_id_type         drive_id,
      unsigned char      flags,
      unsigned char      transfer_mode,
      unsigned char*     scsi_cdb,
      int                cdb_size,
      unsigned char*     status,
      unsigned char*     sense_buffer,
      int                sense_buffer_size,
      unsigned char*     payload,
      int                payload_size,
      unsigned int       timeout
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the drive link capability of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the
 *      link capability of the specified drive from the drive's controller.
 *      The link capability of the drive is written to the location specified
 *      by the link_capability parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      link_capability   - The address where tw_get_drive_link_capability
 *                            will store the drive's link capability.
 *                            This is a bitwise OR'ed combination of
 *                            the following link flags:
 *                              TW_DRIVE_LINK_1_5_GBPS
 *                              TW_DRIVE_LINK_3_0_GBPS
 *                              TW_DRIVE_LINK_6_0_GBPS
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The link capability for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The link capability for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular drive's link capability.
 *        TW_DRIVE_LINK_1_5_GBPS - The drive supports a 1.5 Gbps link speed.
 *        TW_DRIVE_LINK_3_0_GBPS - The drive supports a 3.0 Gbps link speed.
 *        TW_DRIVE_LINK_6_0_GBPS - The drive supports a 6.0 Gbps link speed.
 *
 *    See also:
 *      tw_get_drive_link_status
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      SATA specifications can be found at:
 *         http://www.t13.org/
 *         http://www.serialata.org/
 * </pre>
 *
 */
int tw_get_drive_link_capability (
      tw_drive_id_type      drive_id,
      tw_drive_link_type*   link_capability
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the drive link status of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the
 *      link status of the specified drive from the drive's controller.
 *      The link status of the drive is written to the location specified
 *      by the link_status parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      link_status       - The address where tw_get_drive_link_status
 *                            will store the drive's link status.
 *                            The value is one the following:
 *                              TW_DRIVE_LINK_1_5_GBPS
 *                              TW_DRIVE_LINK_3_0_GBPS
 *                              TW_DRIVE_LINK_6_0_GBPS
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The link status for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The link status for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular drive's link status.
 *        TW_DRIVE_LINK_1_5_GBPS - The drive supports a 1.5 Gbps link speed.
 *        TW_DRIVE_LINK_3_0_GBPS - The drive supports a 3.0 Gbps link speed.
 *        TW_DRIVE_LINK_6_0_GBPS - The drive supports a 6.0 Gbps link speed.
 *
 *    See also:
 *      tw_get_drive_link_capability
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      SATA specifications can be found at:
 *         http://www.t13.org/
 *         http://www.serialata.org/
 * </pre>
 *
 */
int tw_get_drive_link_status (
      tw_drive_id_type      drive_id,
      tw_drive_link_type*   link_status
                             );


/* This routine is deprecated
 *   Use tw_get_drive_link_capability and
 *   tw_get_drive_link_status instead.
 */
int tw_get_sata_link_speed (
      tw_drive_id_type           drive_id,
      tw_sata_link_speed_type*   link_speed
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and above)
 *
 *    Description:
 *      Provides the drive interface and connection type. The API verifies that
 *      the drive ID specified is valid, then requests the interface, connection type
 *      of the specified drive from the drive's controller, the interface type value
 *      is written to the location specified by drive_interface parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      drive_interface   - The address where tw_get_drive_interface will
 *                            store the drive connection type value and is one of:
 *                              TW_DRIVE_INTERFACE_NO_DRIVE
 *                              TW_DRIVE_INTERFACE_SATA_DIRECT_ATTACHED
 *                                This indicates that the drive is of type SATA
 *                                connected directly to controller.
 *                              TW_DRIVE_INTERFACE_SAS_DIRECT_ATTACHED
 *                                This indicates that the drive is of type SAS
 *                                connected directly to controller.
 *                              TW_DRIVE_INTERFACE_SATA_EXPANDER_ATTACHED
 *                                This indicates that the drive is of type SATA
 *                                connected behind expander.
 *                              TW_DRIVE_INTERFACE_SAS_EXPANDER_ATTACHED
 *                                This indicates that the drive is of type SAS
 *                                connected behind expander.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The interface information of the drive
 *                            is returned in the specified parameter.
 *      TW_RET_FAILED     - See the tw_get_error_information() routine to get
 *                          more information about the failed request. 
 *
 *    Remarks:
 *      Applications can use this routine to obtain the drive interface and
 *      connection type of a particular drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_interface (
      tw_drive_id_type           drive_id,
      tw_drive_interface_type*   drive_interface
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and above)
 *
 *    Description:
 *      Provides number of ports available on the specified drive.
 *      The API verifies that the drive ID is valid, then requests
 *      the number of ports of the specified drive, the value is written
 *      to the location specified by ports_supported parameter.
 *
 *    Parameters:
 *      drive_id            - ID of the drive in question.
 *      ports_supported     - The address where tw_get_number_of_ports_supported
 *                              will store the number of drive ports value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of ports of the specified drive
 *                              is available in the specified parameter.
 *      TW_RET_FAILED       - See the tw_get_error_information() routine to get
 *                              more information about the failed request. 
 *
 *    Remarks:
 *      The SAS drive can have more than one SAS port. Applications can use this
 *      routine to get the number of ports available on a particular drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_ports_supported (
      tw_drive_id_type   drive_id,
      int*               ports_supported
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and above)
 *
 *    Description:
 *      Provides number of SAS ports of the specified drive connected to a controller.
 *      The API verifies that the drive ID is valid, then requests the number of ports
 *      of the specified drive connected to the controller, the value is written
 *      to the location specified by ports_connected parameter.
 *
 *    Parameters:
 *      drive_id           - ID of the drive in question.
 *      ports_connected    - The address where tw_get_number_of_ports_connected
 *                             will store the number of drive ports connected value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS     - The number of drive ports connected for the specified drive
 *                             is available in the specified parameter.
 *      TW_RET_FAILED      - See the tw_get_error_information() routine to get
 *                             more information about the failed request. 
 *
 *    Remarks:
 *      The SAS drive can have more than one SAS port. If the drive is dual ported then
 *      there can be two independent paths to the same drive in a configuration which as
 *      one or more than one expander per domain, with controller connected to
 *      more than one domain.
 *      Client application can call this routine to determine number of SAS ports of
 *      the drive connected with the controller.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_ports_connected (
      tw_drive_id_type   drive_id,
      int*               ports_connected
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series(9550 and above)
 *
 *    Description:
 *      Provides the number of reallocated sectors reported by the drive.
 *      The API verifies that the drive ID is valid, then sends the request.
 *      the value is written to the location specified by reallocated_sectors
 *      parameter.
 *
 *    Parameters:
 *      drive_id              - ID of the drive in question.
 *      reallocated_sectors   - The address where tw_get_number_of_reallocated_sectors
 *                                will store the value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of reallocated sectors reported by the drive
 *                              is available in the specified parameter.
 *      TW_RET_FAILED       - See the tw_get_error_information() routine to get
 *                              more information about the failed request. 
 *
 *    Remarks:
 *      Use this routine to read the number of reallocated sectors reported by
 *      a particular drive.(For a SAS drive the number of sectors in the G-list)
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_number_of_reallocated_sectors (
      tw_drive_id_type   drive_id,
      int*               reallocated_sectors
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series(9550 and above)
 *
 *    Description:
 *      Provides the number of power on hours reported by the drive.
 *      The API verifies that the drive ID is valid, then sends the request.
 *      the value is written to the location specified by specified by
 *      power_on_hours parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      power_on_hours    - The address where tw_get_power_on_hours will
 *                            store the value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The number of power on hours reported by the drive
 *                            is available in the specified parameter.
 *      TW_RET_FAILED     - See the tw_get_error_information() routine to get
 *                            more information about the failed request. 
 *
 *    Remarks:
 *      Use this routine to read the number of hours a particular drive
 *      has been powered on. A return value of (-1) means the drive does not
 *      support this feature.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_power_on_hours (
      tw_drive_id_type   drive_id,
      int*               power_on_hours
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series(9550 and above)
 *
 *    Description:
 *      Provides the drive temperature in degree Celsius reported by the drive.
 *      The API verifies that the drive ID is valid, then sends the request.
 *      the value is written to the location specified by temperature parameter.
 *
 *    Parameters:
 *      drive_id         - ID of the drive in question.
 *      temperature      - The address where tw_get_drive_temperature will
 *                           store the value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The drive temperature reported by the drive
 *                           is available in the specified parameter.
 *      TW_RET_FAILED    - See the tw_get_error_information() routine to get
 *                           more information about the failed request. 
 *
 *    Remarks:
 *      The current drive temperature is in degree Celsius.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_temperature (
      tw_drive_id_type   drive_id,
      int*               temperature
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series(9550 and above)
 *
 *    Description:
 *      Provides the drive spindle speed in RPM reported by the drive.
 *      The API verifies that the drive ID is valid, then sends the request.
 *      the value is written to the location specified by spindle_speed parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      spindle_speed     - The address where tw_get_drive_spindle_speed will
 *                            store the value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive spindle speed reported by the drive
 *                            is available in the specified parameter.
 *      TW_RET_FAILED     - See the tw_get_error_information() routine to get
 *                            more information about the failed request. 
 *
 *    Remarks:
 *      Use this routine to read a particular drive's spindle speed(in RPM).
 *      A Zero value means feature not supported.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_spindle_speed (
      tw_drive_id_type   drive_id,
      int*               spindle_speed
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Provides the SAS world wide name (WWN) of the specified drive.
 *      The API verifies that the drive ID is valid, then sends request.
 *      SAS address of the specified drive from the API.
 *      The SAS WWN is written to the location specified by the
 *      sas_wwn parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      sas_wwn           - The address where tw_get_sas_wwn
 *                            will store the SAS WWN.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The SAS WWN for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The SAS WWN for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read a particular drive's SAS address.
 *      For SATA this will be the SAS address of an expander or
 *      an initiator port that the drive is connected to.
 *
 *    See also:
 *      tw_create_drive_id
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_sas_wwn (
      tw_drive_id_type     drive_id,
      tw_sas_wwn_type*     sas_wwn
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides a list of ports from the specified controller.  The API
 *      verifies that the controller ID is valid, then requests the number
 *      and IDs of all ports from the specified controller.  The port IDs
 *      are written to the location specified by port_id_list.  The number
 *      of ports on the specified controller is written to the length
 *      element of the port id list.
 *        
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      port_id_list      - The address where tw_get_port_id_list
 *                              will store the port id list.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The port id list and length are
 *                            available in the specified location.
 *      TW_RET_FAILED     - The port id list is NOT valid and
 *                            the list length is set to zero.
 *
 *    Remarks:
 *      Applications that make calls to other routines in this API that need
 *      port related information must provide the ID of the port of interest.
 *      This routine will provide a list of all ports, by ID, on a particular
 *      controller.  The port ID list is stored in a struct of type
 *      tw_port_id_list_type.  The following fields within the list are of
 *      interest:
 *        list         - An array of <length> port IDs.
 *        length       - The number of port IDs in the list.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_get_unit_id_list
 *      tw_get_subunit_id_list
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_id_list (
      tw_controller_id_type   controller_id,
      tw_port_id_list_type*   port_id_list
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Get the identify state associated with the specified port.
 *      The API verifies that the port ID is valid, then requests
 *      the identify state of the corresponding port.
 *
 *    Parameters:
 *      port_id             - ID of the port of interest.
 *      on_off              - The address where
 *                              tw_get_port_identify_status will
 *                              store the port identify state.
 *                              The port identify state will be one of:
 *                                TW_ON  - drive slot is identified.
 *                                TW_OFF - drive slot is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The identify state for the specified port
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The identify state for the specified port
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to read the identify state of a particular port.
 *
 *    See also:
 *      tw_identify_port
 *      tw_identify_unit
 *      tw_get_unit_identify_status
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_identify_status (
      tw_port_id_type            port_id,
      tw_identify_status_type*   on_off
                                );


/*******************************************************************************
 * Controller Configuration and Management Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      The API verifies that the controller ID is valid, then:
 *        1) Removes incomplete units and the corresponding drives (9000 only).
 *        2) Tries to add drives to the empty ports on the controller
 *           (including the empty ports that resulted from step 1).
 *        3) Calls tw_import_unit with the drives added in step 2.
 *        4) Returns a list of:
 *             Newly imported units (in the new_units  parameter).
 *             Newly added drives that are not part of the newly imported units
 *             (in the new_drives parameter).
 *      Note only single drive units can be imported on 7000 & 8000 controllers.
 *
 *    Parameters:
 *      controller_id       - ID of the controller to rescan.
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the imported unit.
 *                              Must be one of:
 *                                TW_DO_NOT_NOTIFY_OS_OF_IMPORTED_UNIT
 *                                TW_NOTIFY_OS_OF_IMPORTED_UNIT
 *      new_units           - The address of a list of newly imported unit IDs.
 *      new_drives          - The address of a list of newly added drive IDs
 *                              of drives found but are not part of a new unit.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The list of newly imported units and
 *                              newly added drives is available.
 *      TW_RET_FAILED       - The list of newly imported units and
 *                              newly added drives is NOT available.
 *
 *    Remarks:
 *      Use this routine after physically attaching drives.  If the attached
 *      drives make up one or more units then those units will be imported.
 *      If the attached drives are new drives, they will be added.
 *
 *    See also:
 *      tw_remove_drive
 *      tw_add_drive
 *      tw_export_unit
 *      tw_import_unit
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_rescan_controller (
      tw_controller_id_type    controller_id,
      unsigned char            make_os_aware,
      tw_unit_id_list_type*    new_units,
      tw_drive_id_list_type*   new_drives
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Sets the background rebuild rate of the specified 3ware controller.
 *      The API verifies that the controller ID and background rebuild rate
 *      are valid, then sets the background rebuild rate on the specified
 *      controller with the value specified by the background_rebuild_rate
 *      parameter.
 *
 *    Parameters:
 *      controller_id           - ID of the controller in question.
 *      background_rebuild_rate - The value tw_set_background_rebuild_rate 
 *                                will use to set the controller background 
 *                                rebuild rate.  Must be one of:
 *                                  TW_BACKGROUND_TASK_RATE_LOW
 *                                  TW_BACKGROUND_TASK_RATE_MED_LOW
 *                                  TW_BACKGROUND_TASK_RATE_MEDIUM
 *                                  TW_BACKGROUND_TASK_RATE_MED_HI
 *                                  TW_BACKGROUND_TASK_RATE_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The background rebuild rate for the specified
 *                              controller is set with the specified value.
 *      TW_RET_FAILED       - The background rebuild rate for the specified
 *                              controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to write the rate at which a particular controller
 *      verifies redundant units (RAID 1, RAID 5, RAID 10, RAID 50).
 *      The task rate has the following effects on performance during a
 *      rebuild operation:
 *        TW_BACKGROUND_TASK_RATE_LOW       Lowest   task rate, highest IO rate
 *        TW_BACKGROUND_TASK_RATE_MED_LOW   Lower    task rate, higher  IO rate
 *        TW_BACKGROUND_TASK_RATE_MEDIUM    Balanced task and IO rates
 *        TW_BACKGROUND_TASK_RATE_MED_HI    Higher   task rate, lower   IO rate
 *        TW_BACKGROUND_TASK_RATE_HIGH      Highest  task rate, lowest  IO rate
 *
 *    See also:
 *      tw_get_background_rebuild_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_background_rebuild_rate (
      tw_controller_id_type           controller_id,
      tw_background_task_rate_type*   background_rebuild_rate
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Sets the Background Task Activity Policy(BTAP) setting for rebuild task
 *      on the specified 3ware controller. The API verifies that the controller ID is valid,
 *      then sets the BTAP setting for the specified controller.
 *
 *    Parameters:
 *      controller_id             - ID of the controller in question.
 *      btap_policy               - The value tw_set_background_rebuild_activity_policy 
 *                                    will use to set the controller BTAP Policy.
 *                                    Must be one of:
 *                                      TW_BTAP_POLICY_ADAPTIVE
 *                                      TW_BTAP_POLICY_LOW_LATENCY
 *
 *    Return Values:
 *      TW_RET_SUCCESS            - Successful in setting the BTAP policy for the
 *                                    specified controller.
 *      TW_RET_FAILED             - Failed to set the BTAP policy for the specified controller.
 *                                    For error details, see tw_get_error_information()
 *
 *    Remarks:
 *      Use this routine to write the background task activity policy(BTAP) setting for rebuild task
 *      on a particular controller. Use BTAP setting along with the background rebuild rate for customers
 *      that have requirement for lower latency of host read commands. 
 *
 *        When background tasks are active (Rebuild, Verify, Init etc) increases latency significantly,
 *      if the task rate is set to lower IO rate. Using this feature, setting it to low latency mode
 *      will throttle the background task and allow the host read commands to complete within 100ms or less.
 *
 *    See also:
 *      tw_get_background_rebuild_activity_policy
 *      tw_get_background_rebuild_rate
 *      tw_set_background_rebuild_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_background_rebuild_activity_policy (
      tw_controller_id_type    controller_id,
      tw_btap_policy_type      btap_policy
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Sets the background verify rate of the specified 3ware controller.
 *      The API verifies that the controller ID and background verify rate
 *      are valid, then sets the background verify rate on the specified
 *      controller with the value specified by the background_verify_rate
 *      parameter.
 *
 *    Parameters:
 *      controller_id          - ID of the controller in question.
 *      background_verify_rate - The address tw_set_background_verify_rate
 *                               will use to set the controller background
 *                               verify rate.  Value must be one of:
 *                                 TW_BACKGROUND_TASK_RATE_LOW
 *                                 TW_BACKGROUND_TASK_RATE_MED_LOW
 *                                 TW_BACKGROUND_TASK_RATE_MEDIUM
 *                                 TW_BACKGROUND_TASK_RATE_MED_HI
 *                                 TW_BACKGROUND_TASK_RATE_HIGH
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The background verify rate for the specified
 *                              controller is set with the specified value.
 *      TW_RET_FAILED       - The background verify rate for the specified
 *                              controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to write the rate at which a particular controller
 *      verifies units.  The task rate has the following effects on performance
 *      during a verify operation:
 *        TW_BACKGROUND_TASK_RATE_LOW       Lowest   task rate, highest IO rate
 *        TW_BACKGROUND_TASK_RATE_MED_LOW   Lower    task rate, higher  IO rate
 *        TW_BACKGROUND_TASK_RATE_MEDIUM    Balanced task and IO rates
 *        TW_BACKGROUND_TASK_RATE_MED_HI    Higher   task rate, lower   IO rate
 *        TW_BACKGROUND_TASK_RATE_HIGH      Highest  task rate, lowest  IO rate
 *
 *    See also:
 *      tw_set_background_verify_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_background_verify_rate (
      tw_controller_id_type           controller_id,
      tw_background_task_rate_type*   background_verify_rate
                                  );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and above)
 *
 *    Description:
 *      Sets the Background Task Activity Policy(BTAP) setting for verify task
 *      on the specified 3ware controller. The API verifies that the controller ID is valid,
 *      then sets the BTAP setting for the specified controller.
 *
 *    Parameters:
 *      controller_id             - ID of the controller in question.
 *      btap_policy               - The value tw_set_background_verify_activity_policy 
 *                                    will use to set the controller BTAP Policy.
 *                                    Must be one of:
 *                                      TW_BTAP_POLICY_ADAPTIVE
 *                                      TW_BTAP_POLICY_LOW_LATENCY
 *
 *    Return Values:
 *      TW_RET_SUCCESS            - Successful in setting the BTAP policy for the
 *                                    specified controller.
 *      TW_RET_FAILED             - Failed to set the BTAP policy for the specified controller.
 *                                    For error details, see tw_get_error_information()
 *
 *    Remarks:
 *      Use this routine to write the background task activity policy(BTAP) setting for verify task
 *      on a particular controller. Use BTAP setting along with the background verify rate for customers
 *      that have requirement for lower latency of host read commands. 
 *
 *        When background tasks are active (Rebuild, Verify, Init etc) increases latency significantly,
 *      if the task rate is set to lower IO rate. Using this feature, setting it to low latency mode
 *      will throttle the background task and allow the host read commands to complete within 100ms or less.
 *
 *    See also:
 *      tw_get_background_verify_activity_policy
 *      tw_get_background_verify_rate
 *      tw_set_background_verify_rate
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_background_verify_activity_policy (
      tw_controller_id_type    controller_id,
      tw_btap_policy_type      btap_policy
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets the test enable flags of the specified 3ware controller.
 *      The API verifies that the controller ID is valid, then sets the
 *      test enable flags on the specified controller with the value 
 *      specified by the test_enable_flags parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      test_enable_flags   - The address tw_get_selftest_flags will use
 *                              to set this controller's test enable flags.
 *                              The value must be a bitwise OR'ed combination
 *                              of the following selftest flags:
 *                                TW_SELFTEST_ENABLE_UPGRADE_DMA_MODE
 *                                TW_SELFTEST_ENABLE_CHECK_SMART_THRESHOLD
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The selftest flags for the specified controller
 *                              are set with the specified value.
 *      TW_RET_FAILED       - The selftest flags for the specified controller
 *                              are NOT set with the specified value.
 *
 *    Remarks:
 *      Use this routine to enable and disable selftests on a particular
 *      controller.
 *
 *    See also:
 *      tw_get_selftest_flags
 *      tw_get_selftest_task_item
 *      tw_set_selftest_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_selftest_flags (
      tw_controller_id_type       controller_id,
      tw_selftest_enable_flags*   test_enable_flags
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the rebuild task schedule on the specified controller.
 *    The API verifies that the controller ID is valid, then enables the
 *    rebuild task on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The rebuild task is enabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The rebuild task state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      Use this routine to enable the state of a particular controller's
 *      rebuild schedule.  The rebuild task will perform rebuild operations
 *      according to the rebuild schedule.
 *
 *    See also:
 *      tw_get_rebuild_schedule_state
 *      tw_disable_rebuild_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_rebuild_task_schedule ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the rebuild task schedule on the specified controller.
 *    The API verifies that the controller ID is valid, then disables the
 *    rebuild task on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The rebuild task is disabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The rebuild task state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      Use this routine to disable the state of a particular controller's
 *      rebuild schedule.  The rebuild task will perform rebuild operations
 *      only through rebuild_unit commands.
 *
 *    See also:
 *      tw_get_rebuild_schedule_state
 *      tw_enable_rebuild_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_rebuild_task_schedule ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the verify task schedule on the specified controller.
 *    The API verifies that the controller ID is valid, then enables the
 *    verify task on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The verify task is enabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The verify task state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      Use this routine to enable the state of a particular controller's
 *      verify schedule.  The verify task will perform verify operations
 *      according to the verify schedule.
 *
 *    See also:
 *      tw_get_verify_schedule_state
 *      tw_disable_verify_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_verify_task_schedule ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the verify task schedule on the specified controller.
 *    The API verifies that the controller ID is valid, then disables the
 *    verify task on the specified controller.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The verify task is disabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The verify task state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      Use this routine to disable the state of a particular controller's
 *      verify schedule.  The verify task will perform verify operations
 *      only through verify_unit commands.
 *
 *    See also:
 *      tw_get_verify_schedule_state
 *      tw_enable_verify_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_verify_task_schedule ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Sets the start time and duration of the specified rebuild task on the
 *      specified 3ware controller.  The API verifies that the controller ID,
 *      task number, start time and duration are valid, then sets the
 *      start time and duration of the specified controller's task.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The rebuild task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The specified rebuild task's start time.
 *                              Value must be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *      duration            - The specified rebuild task's duration.
 *                              Value must be in the range of:
 *                                TW_MIN_TEST_DURATION ..
 *                                TW_MAX_TEST_DURATION
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified rebuild task
 *                              is set with the specified values.
 *      TW_RET_FAILED       - The schedule for the specified rebuild task
 *                              is NOT set with the specified values.
 *
 *    Remarks:
 *      Use this routine to write a particular rebuild task schedule to a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_get_rebuild_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_rebuild_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type       start_time,
      tw_task_item_type       duration
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Sets the start time and duration of the specified verify task on the
 *      specified 3ware controller.  The API verifies that the controller ID,
 *      task number, start time and duration are valid, then sets the
 *      start time and duration of the specified controller's task.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The verify task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The specified verify task's start time.
 *                              Value must be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *      duration            - The specified verify task's duration.
 *                              Value must be in the range of:
 *                                TW_MIN_TEST_DURATION ..
 *                                TW_MAX_TEST_DURATION
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified verify task
 *                              is set with the specified values.
 *      TW_RET_FAILED       - The schedule for the specified verify task
 *                              is NOT set with the specified values.
 *
 *    Remarks:
 *      Use this routine to write a particular verify task schedule to a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_get_verify_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_verify_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type       start_time,
      tw_task_item_type       duration
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Sets the preferred start time of the basic verify task
 *      on the specified 3ware controller.  The API verifies that the
 *      controller ID, preferred start time are valid, then sets the
 *      preferred verify start time of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      start_time          - The specified preferred verify start time.
 *                              Value must be in the range of:
 *                                TW_MIN_PREFERRED_START_TIME ..
 *                                TW_MAX_PREFERRED_START_TIME
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The preferred verify start time
 *                              is set with the specified values.
 *      TW_RET_FAILED       - The preferred verify start time
 *                              is NOT set with the specified values
 *
 *    Remarks:
 *      Use this routine to write a preferred verify start time to a
 *      particular controller. The preferred start time is in minutes and
 *      only represents the time within a given week.
 *      For example:   0 = SUNDAY midnight,  1500 = MONDAY 1:00am
 *      
 *      NOTE:
 *      The controller runs verify task at preferred verify start time
 *      once a week, if the controller's verify task schedule setting is
 *      disabled through the tw_disable_verify_task_schedule API.
 *
 *    See also:
 *      tw_get_verify_preferred_time
 *      tw_disable_verify_task_schedule
 *      tw_enable_verify_task_schedule
 *      tw_get_verify_task_item
 *      tw_set_verify_task_item
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_verify_preferred_time (
      tw_controller_id_type     controller_id,
      tw_preferred_time_type    start_time
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Sets the start time and test enable flags of the specified selftest
 *      task on the specified 3ware controller.  The API verifies that the
 *      controller ID, task number, start time and test enable flags are valid,
 *      then sets the start time and test enable flags of the specified
 *      controller's task.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_number         - The selftest task in question.
 *                              Must be one of:
 *                                TW_TASK_0
 *                                TW_TASK_1
 *                                TW_TASK_2
 *                                TW_TASK_3
 *                                TW_TASK_4
 *                                TW_TASK_5
 *                                TW_TASK_6
 *      start_time          - The specified selftest task's start time.
 *                              Must be in the range of:
 *                                TW_MIN_TEST_START_TIME ..
 *                                TW_MAX_TEST_START_TIME
 *      test_enable_flags   - The specified selftest task's test enable flags.
 *                              This is a bitwise OR'ed combination of the
 *                              following selftest flags:
 *                                TW_SELFTEST_ENABLE_UPGRADE_DMA_MODE
 *                                TW_SELFTEST_ENABLE_CHECK_SMART_THRESHOLD
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The schedule for the specified selftest task
 *                              is set with the specified values.
 *      TW_RET_FAILED       - The schedule for the specified selftest task
 *                              is NOT set with the specified values.
 *
 *    Remarks:
 *      Use this routine to write a particular selftest task schedule to a
 *      particular controller.  Note that tasks can be scheduled throughout
 *      the week.  Task "aliases" TW_SUNDAY .. TW_SATURDAY may be used when
 *      tasks are scheduled on a daily basis.
 *
 *    See also:
 *      tw_get_selftest_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_selftest_task_item (
      tw_controller_id_type   controller_id,
      tw_task_item_type       task_number,
      tw_task_item_type       start_time
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets the task schedule of the specified 3ware controller.
 *      The API verifies that the controller ID and task schedule
 *      are valid, then sets the task schedule on the specified
 *      controller with the value specified by the task_schedule
 *      parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      task_schedule       - The address tw_set_task_schedule will use
 *                              to set the controller task schedule.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The task schedule for the specified
 *                              controller is set with the specified value.
 *      TW_RET_FAILED       - The task schedule for the specified
 *                              controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to write an entire task schedule to a particular 
 *      controller.  This includes rebuild, verify and selftest tasks.
 *
 *    See also:
 *      tw_get_task_schedule
 *      tw_get_rebuild_task_item
 *      tw_get_verify_task_item
 *      tw_get_selftest_task_item
 *      tw_set_rebuild_task_item
 *      tw_set_verify_task_item
 *      tw_set_selftest_task_item
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_task_schedule (
      tw_controller_id_type    controller_id,
      tw_task_schedule_type*   task_schedule
                         );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *        Sets the number of spinups allowed by the specified 3ware controller.
 *      The API verifies that the controller ID and spinups allowed are valid,
 *      then uses the value specified by the spinups_allowed parameter to set
 *      the number of drives that can be spun up, before a stagger time wait
 *      is required on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      spinups_allowed     - The value tw_set_spinups_allowed will
 *                              use to set the number of spinups allowed
 *                              by the specified controller.
 *                              Must be in the range of:
 *                                TW_MIN_SPINUPS_ALLOWED..
 *                                TW_MAX_SPINUPS_ALLOWED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The number of spinups allowed for the specified
 *                              controller is set with the specified value.
 *      TW_RET_FAILED       - The number of spinups allowed for the specified
 *                              controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to write the number of drives that will spinup,
 *      at one time, on a particular controller.  Systems may need to
 *      spinup a few drives at a time in order to reduce demands on the
 *      system power supply.
 *
 *    See also:
 *      tw_get_spinups_allowed
 *      tw_get_stagger_time
 *      tw_set_stagger_time
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_spinups_allowed (
      tw_controller_id_type     controller_id,
      tw_spinups_allowed_type   spinups_allowed
                           ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets the stagger time of the specified 3ware controller.  The API
 *      verifies that the controller ID and stagger time are valid, then
 *      uses the value specified by the stagger_time parameter to set the
 *      time to delay before starting the next set of drive spinups, on the
 *      specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      stagger_time        - The value tw_set_stagger_time will
 *                              use to set the controller stagger time.
 *                              Must be in the range of:
 *                                TW_MIN_STAGGER_TIME..
 *                                TW_MAX_STAGGER_TIME
 *                              Units are in seconds.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The stagger time for the specified controller
 *                              is set with the specified value.
 *      TW_RET_FAILED       - The stagger time for the specified controller
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to write the delay between drive groups that will
 *      spinup, at one time, on a particular controller.  Systems may need
 *      to spinup a few drives at a time in order to reduce demands on the
 *      system power supply.
 *
 *    See also:
 *      tw_get_spinups_allowed
 *      tw_get_stagger_time
 *      tw_set_spinups_allowed
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_stagger_time (
      tw_controller_id_type   controller_id,
      tw_stagger_time_type    stagger_time
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets the rollcall pause time of the specified 3ware controller.
 *      The API verifies that the controller ID and rollcall pause values are valid,
 *      then writes the value on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      rollcall_pause      - The value tw_set_rollcall_pause will
 *                              use to set the controller rollcall pause time.
 *                              Must be in the range of:
 *                                TW_MIN_ROLLCALL_PAUSE_TIME..
 *                                TW_MAX_ROLLCALL_PAUSE_TIME
 *                              Units are in seconds.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The rollcall pause for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The rollcall pause for the specified controller
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to set the initial pause or delay(in seconds) before the drive
 *      rollcall/spinup begins on a particular controller.
 *      Systems may need to adjust this value in order to balance the demands
 *      on the system power supply.
 *
 *    See also:
 *      tw_get_rollcall_pause
 *      tw_get_spinups_allowed
 *      tw_set_spinups_allowed
 *      tw_get_stagger_time
 *      tw_set_stagger_time
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_rollcall_pause (
      tw_controller_id_type    controller_id,
      tw_rollcall_pause_type   rollcall_pause
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets JBOD policy of the specified controller to make JBOD units
 *      available to the operating system.  The API verifies that the
 *      controller ID is valid, then sets JBOD policy of the specified
 *      controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The JBOD policy of the specified controller makes
 *                            JBOD units available to the operating system.
 *      TW_RET_FAILED     - The JBOD policy of the specified controller
 *                            is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable the JBOD policy on a particular controller.
 *      This routine will write a value of TW_POLICY_ENABLED which indicates
 *      that JBOD units are available to the operating system.
 *
 *    See also:
 *      tw_get_jbod_policy
 *      tw_disable_jbod_online
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_jbod_online ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets JBOD policy of the specified controller to make JBOD units
 *      unavailable to the operating system.  The API verifies that the
 *      controller ID is valid, then sets JBOD policy of the specified
 *      controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The JBOD policy of the specified controller makes
 *                            JBOD units unavailable to the operating system.
 *      TW_RET_FAILED     - The JBOD policy of the specified controller
 *                            is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable the JBOD policy on a particular controller.
 *      This routine will write a value of TW_POLICY_DISABLED which indicates
 *      that JBOD units are not available to the operating system.
 *
 *    See also:
 *      tw_get_jbod_policy
 *      tw_enable_jbod_online
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_jbod_online ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9500S only)
 *
 *    Description:
 *      Sets the disable_cache_on_degrade policy of the specified controller.  
 *      The API verifies the controller ID is valid, then sets the cache policy
 *      of the specified controller.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The cache policy of the specified controller
 *                              disables the controller cache if any redundant
 *                              unit becomes degraded.
 *      TW_RET_FAILED       - The cache policy of the specified controller
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable the disable_cache_on_degrade policy on a 
 *      particular controller. This routine will write a value of TW_POLICY_ENABLED 
 *      which indicates that the controller cache will be disabled if any of the 
 *      controller's redundant units degrade.  The controller cache is re-enabled when
 *      no redundant unit is degraded.
 *
 *    See also:
 *      tw_get_cache_policy
 *      tw_use_unit_cache_states
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_cache_on_unit_degrade ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                         9000 series (9500S only)
 *
 *    Description:
 *      Disables the disable_cache_on_degrade policy of the specified controller.  
 *      The API verifies the controller ID is valid, then sets the cache policy
 *      of the specified controller.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The cache policy of the specified controller
 *                              uses the unit cache states.
 *      TW_RET_FAILED       - The cache policy of the specified controller
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable the cache policy on a particular controller.
 *      This routine will write a value of TW_POLICY_DISABLED which indicates
 *      that the controller will not disable the cache on degrade each units cache 
 *      state will be maintained if the array becomes disabled.
 *
 *    See also:
 *      tw_get_cache_policy
 *      tw_disable_cache_on_unit_degrade
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_use_unit_cache_states ( tw_controller_id_type   controller_id );


/*
 This Function is not supported.
 */
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Marks the specified unit as bootable.
 *      The API verifies that the boot unit ID is valid, then sets
 *      the specified unit's controller with the value specified
 *      by the boot_unit_id parameter.
 *
 *    Parameters:
 *      boot_unit_id        - The unit ID tw_set_boot_unit will use
 *                              to set the unit's controller boot unit.
 *                              Must be
 *                                TW_NULL_UNIT
 *                              or in the range of:
 *                                TW_FIRST_UNIT..
 *                                TW_MAX_UNITS
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The boot unit of the specified unit's controller
 *                              is set with the specified value.
 *      TW_RET_FAILED       - The boot unit of the specified unit's controller
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to write which unit a particular controller will
 *      present as a system boot target.  A value of TW_NULL_UNIT indicates
 *      that no unit is available as a system boot target.
 *
 *    See also:
 *      tw_get_boot_unit
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_boot_unit ( tw_unit_id_type   boot_unit_id ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables AV mode on the specified controller.
 *      The API verifies that the controller ID is valid, then enables the
 *      AV mode state on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The AV mode is enabled for the
 *                              specified controller.
 *      TW_RET_FAILED       - The AV mode is unchanged for the
 *                              specified controller.
 *
 *    Remarks:
 *      Use this routine to enable a particular controller's AV mode.
 *      In addition, it disables LBA reordering. Use tw_enable_avmode_2 to 
 *      enable AV mode only.
 *
 *    See also:
 *      tw_get_avmode
 *      tw_enable_avmode_2
 *      tw_disable_avmode
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_avmode ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables AV mode on the specified controller.
 *      The API verifies that the controller ID is valid, then enables the
 *      AV mode state on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The AV mode is enabled for the
 *                              specified controller.
 *      TW_RET_FAILED       - The AV mode is unchanged for the
 *                              specified controller.
 *
 *    Remarks:
 *      Use this routine to enable a particular controller's AV mode.
 *
 *    See also:
 *      tw_get_avmode
 *      tw_disable_avmode
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_avmode_2 ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables AV mode on the specified controller.
 *      The API verifies that the controller ID is valid, then disables the
 *      AV mode state on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The AV mode is disabled for the
 *                              specified controller.
 *      TW_RET_FAILED       - The AV mode is unchanged for the
 *                              specified controller.
 *
 *    Remarks:
 *      Use this routine to disable a particular controller's AV mode.
 *
 *    See also:
 *      tw_get_avmode
 *      tw_enable_avmode
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_avmode ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Shutdown IO to units on the specified controller.  
 *      Ensures redundant units are shutdown cleanly.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - IO to the specified controller is shutdown.
 *      TW_RET_FAILED       - IO to the specified controller is NOT shutdown.
 *
 *    Remarks:
 *      Use this routine to shutdown IO in a controlled and orderly manner.
 *      This is particularly useful with applications that use raw disk IO
 *      such as video, database and benchmark applications.
 *
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_shutdown ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *
 *    Description:
 *      Pauses media scan operation on the specified controller. The API
 *      verifies that the Controller ID is valid  then pauses media scan
 *      operations on the specified controller.  Controller will retain
 *      state of progress.  State of progress does not survive reboot.
 *
 *    Parameters:
 *      controller_id       - ID of the controller of interest.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Media scan operation paused
 *                              on the specified controller.
 *      TW_RET_FAILED       - Media scan operation unchanged
 *                              on the specified controller.
 *
 *    Remarks:
 *      Use this routine to pause media scan operations.
 *      Use the tw_resume_mediascan to resume media scan operations.
 *
 *    See also:
 *      tw_resume_mediascan
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_pause_mediascan ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *
 *    Description:
 *      Resumes Media Scan on the specified controller. The API verifies that
 *      the Controller ID is valid,  then resumes media scan operations on the
 *      specified controller.  All disks on the specified controller will be
 *      scanned sequentially.  Controller will retain state of progress.
 *      State of progress does not survive reboot.
 *
 *    Parameters:
 *      controller_id       - ID of the controller of interest.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Rebuild operation paused
 *                              on the specified unit.
 *      TW_RET_FAILED       - Rebuild operation unchanged
 *                              on the specified unit.
 *
 *    Remarks:
 *      Use this routine to resume a media scan operations.
 *      Use the tw_pause_mediascan to pause media scan operations.
 *
 *    See also:
 *      tw_pause_mediascan
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_resume_mediascan ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Writes the rebuild policy to the specified controller.  The API
 *      verifies the controller ID, then writes the specified rebuild policy
 *      to the specified controller.
 * 
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      auto_rebuild_policy - The value tw_set_auto_rebuild_policy
 *                              will write to the specified controller's
 *                              rebuild policy.  Value MUST be 
 *                                TW_POLICY_AUTO_REBUILD_OFF,
 *                                TW_POLICY_AUTO_REBUILD_ALL
 *                                   or a bitwise OR'ed combination of:
 *                                TW_POLICY_AUTO_REBUILD_CBOD
 *                                TW_POLICY_AUTO_REBUILD_JBOD
 *                                TW_POLICY_AUTO_REBUILD_FAILED_DISK
 *
 *                            NOTE: TW_POLICY_AUTO_REBUILD_ALL includes 5 flags
 *                                    that are not presently defined.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified rebuild policy is written
 *                              to the specified controller.
 *      TW_RET_FAILED       - The rebuild policy is unchanged on the
 *                              specified controller.
 *
 *    Remarks:
 *      Use this routine to write the rebuild policy on a particular controller.
 *      A value of TW_POLICY_AUTO_REBUILD_OFF indicates that the controller
 *      will NOT start a rebuild operation when a unit, with a redundant
 *      configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_CBOD indicates that the controller
 *      will start a rebuild operation, using any available CBOD, when a unit,
 *      with a redundant configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_JBOD indicates that the controller
 *      will start a rebuild operation, using any available JBOD, when a unit,
 *      with a redundant configuration, degrades if the controller's JBOD
 *      policy is disabled (TW_POLICY_DISABLED).
 *
 *      A value of TW_POLICY_AUTO_REBUILD_FAILED_DISK indicates that the
 *      will start a rebuild operation, using the original failing disk,
 *      when a unit, with a redundant configuration, degrades.
 *
 *      A value of TW_POLICY_AUTO_REBUILD_ALL indicates that the controller
 *      will rebuild using all of the available options listed above.
 *
 *    See also:
 *      tw_get_auto_rebuild_policy
 *      tw_get_jbod_policy
 *      tw_disable_jbod_online
 *      tw_enable_jbod_online
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_auto_rebuild_policy (
          tw_controller_id_type         controller_id,
          tw_auto_rebuild_policy_type   auto_rebuild_policy
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the LUN policy of the specified controller.
 *      The API verifies that the controller ID is valid, then
 *      sets the LUN policy of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The LUN policy of the specified controller
 *                            is enabled.
 *      TW_RET_FAILED     - The LUN policy of the specified controller
 *                            is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable the LUN policy on a particular controller.
 *      This routine will write a value of TW_POLICY_ENABLED which indicates
 *      that the controller will create multiple LUNs per unit.  Each lun will
 *      have a maximum capacity of lun_size as specified by tw_get_lun_size.
 *
 *    See also:
 *      tw_get_lun_policy 
 *      tw_disable_lun_policy
 *      tw_get_lun_size
 *      tw_set_lun_size
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_lun_policy ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the LUN policy of the specified controller.
 *      The API verifies that the controller ID is valid, then
 *      sets the LUN policy of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The LUN policy of the specified controller
 *                            is disabled.
 *      TW_RET_FAILED     - The LUN policy of the specified controller
 *                            is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable the LUN policy on a particular controller.
 *      This routine will write a value of TW_POLICY_DISABLED which indicates
 *      that the controller will create exactly one LUN per unit.
 *
 *    See also:
 *      tw_get_lun_policy 
 *      tw_enable_lun_policy
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_lun_policy ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Writes the lun size of the specified controller.  The API verifies
 *      that the controller ID and lun size are valid, then writes the specified
 *      lun size to the specified controller.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      lun size          - The value tw_get_unit_lun size will
 *                              write as the controller's lun size.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The specified lun size is written
 *                            to the specified controller.
 *      TW_RET_FAILED     - The specified lun size is NOT written
 *                            to the specified controller.
 *
 *    Remarks:
 *      Each controller has a lun size which is used during unit creation
 *      if the lun policy is enabled.  See tw_enable_lun_policy for more
 *      information.  Applications can use this routine to write a particular
 *      controller's lun size.
 *
 *    See also:
 *      tw_set_lun_size
 *      tw_get_lun_policy 
 *      tw_enable_lun_policy
 *      tw_disable_lun_policy
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_lun_size (
      tw_controller_id_type   controller_id,
      tw_lun_size_type        lun_size
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the drive LBA reordering on the specified controller.  The API verifies
 *      that the controller ID is valid, then enables this feature. This is controller wide
 *      setting applies all the drives connected to the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller on which to enable LBA reordering.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified controller LBA reordering is enabled.
 *      TW_RET_FAILED       - The specified controller LBA reordering is NOT enabled.
 *
 *    Remarks:
 *      Use this routine to enable a particular controller's LBA reordering.
 *
 *    See also:
 *      tw_get_lba_reorder_state
 *      tw_disable_lba_reorder         
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_lba_reorder ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the drive LBA reordering on the specified controller.  The API verifies
 *      that the controller ID is valid, then disables this feature. This is controller wide
 *      setting applies all the drives connected to the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller on which to disable LBA reordering.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified controller LBA reordering is disabled.
 *      TW_RET_FAILED       - The specified controller LBA reordering is NOT disabled.
 *
 *    Remarks:
 *      Use this routine to disable a particular controller's LBA reordering.
 *
 *    See also:
 *      tw_get_lba_reorder_state
 *      tw_enable_lba_reorder         
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_lba_reorder ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables drive detection for the ports specified upon drive hot plugged.
 *      The API first verifies that the controller ID is valid and then enables this feature.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      port_id_list        - list of port IDs on which to enable the drive detection.
 *
 *    Return Values:
 *        TW_RET_SUCCESS    - The drive detection is enabled.
 *        TW_RET_FAILED     - The drive detection policy is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable drive detection upon drive hot plugged on a particular controller.
 *
 *    See also:
 *      tw_disable_auto_spinup
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_auto_spinup (
      tw_controller_id_type   controller_id,
      tw_port_id_list_type*   port_id_list
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      disables drive detection for the ports specified upon drive hot plugged.
 *      The API first verifies that the controller ID is valid and then disables this feature.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      port_id_list        - list of port IDs on which to disable the drive detection.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive detection is disabled.
 *      TW_RET_FAILED     - The drive detection policy is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable drive detection upon drive hot plugged on a particular controller.
 *
 *    See also:
 *      tw_enable_auto_spinup
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_auto_spinup (
      tw_controller_id_type   controller_id,
      tw_port_id_list_type*   port_id_list
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Sets the drive coercion factor for the specified controller.
 *        The API verifies that the controller ID is valid, then sends request to set the
 *        drive coercion factor for the specified controller.
 *
 *      Parameters:
 *        controller_id      - ID of the controller in question.
 *        coercion_factor    - The drive coercion factor
 *                              must be in the range of:
 *                                TW_MIN_COERCION_FACTOR ...
 *                                TW_MAX_COERCION_FACTOR
 *                              The drive coercion factor is in multiple of GB. i.e. 
 *                                   Drive coercion factor 1  --> 1 GB. 
 *                                   Drive coercion factor 2  --> 2 GB. 
 *                                   Drive coercion factor 3  --> 3 GB. and so on
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - Successful in setting the drive coercion factor
 *                              for the specified controller.
 *        TW_RET_FAILED     - Failed to set the drive coercion factor
 *                              for the specified controller. To get details on error see
 *                              tw_get_error_information().
 *
 *      Remarks:
 *        Each controller has a drive coercion factor which is used during unit creation.
 *        The raw capacity of the drive is truncated to the smallest number of sectors that
 *        is larger than or equal to an even multiple of drive coersion factor.
 *        The controller default setting is 5 GB for a drive with raw capacity >= 45 GB.
 *                                          1 GB for a drive with raw capacity <  45 GB.
 *
 *      See also:
 *        tw_get_drive_coercion_factor
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_set_drive_coercion_factor (
      tw_controller_id_type        controller_id,
      tw_coercion_factor_type      coercion_factor
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Writes the SCSI mode page control setting of the specified controller.
 *
 *    Parameters:
 *      controller_id        - ID of the controller in question.
 *      mode_control         - The SCSI mode page control value,
 *                               must be one of the following:
 *                                 TW_SCSI_MODEPAGE_CONTROL_HONOR
 *                                 TW_SCSI_MODEPAGE_CONTROL_IGNORE_WCE
 *
 *    Remarks:
 *      Use this routine to write the SCSI mode page control setting of
 *      a particular controller. This setting modifies the firmware behavior of
 *      host SCSI mode sense & mode select commands.  
 *
 *      A value of TW_SCSI_MODEPAGE_CONTROL_IGNORE_WCE indicates that the controller firmware
 *      ignores OS request via the SCSI Mode Select command to disable/enable write cache.
 *      This means write cache setting is only changeable through unit cache configuration routines.
 *      The complement of this is TW_SCSI_MODEPAGE_CONTROL_HONOR.
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - Successful in setting the SCSI mode page control
 *                            for the specified controller
 *      TW_RET_FAILED    - Failed to set the SCSI mode page control
 *                            for the specified controller. See tw_get_error_information() to get error details.
 *
 *    See also:
 *      tw_get_scsi_modepage_control
 *      tw_enable_unit_write_cache
 *      tw_disable_unit_write_cache
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_scsi_modepage_control (
      tw_controller_id_type        controller_id,
      tw_modepage_control_type     mode_control
                               );




/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      This routine is obsolete.
 *
 * </pre>
 *
 */
int tw_bbu_initialize_battery ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Tests the capacity of the battery on the battery backup unit.
 *      The API verifies that the controller ID is valid, and the
 *      battery backup unit is present, then tests the battery.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The battery for the specified controller
 *                              is being tested.
 *      TW_RET_FAILED       - The battery for the specified controller
 *                              is NOT being tested.
 *
 *    Remarks:
 *      Use this routine to test the battery on the battery backup unit.
 *      The battery should be tested periodically to measure the effective
 *      backup time it can provide.  This test can take up to 24 hours to
 *      complete.  During this test the data protection feature provided
 *      by the battery backup unit is disabled.
 *
 *    See also:
 *      tw_bbu_initialize_battery
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_test_battery ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the battery backup unit on the specified 3ware controller.
 *      The API verifies that the controller ID is valid and the battery backup
 *      unit is present, then enables the battery backup unit on the specified
 *      controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The battery backup unit for the
 *                              specified controller is enabled.
 *      TW_RET_FAILED       - The battery backup unit for the
 *                              specified controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable a particular controller's battery backup
 *      unit.  An enabled BBU will be used by the controller in the event
 *      of a power failure.
 *
 *    See also:
 *      tw_bbu_disable
 *      tw_get_bbu_online_state
 *      tw_get_bbu_status_summary
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_enable ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the battery backup unit on the specified 3ware controller.
 *      The API verifies that the controller ID is valid and the battery backup
 *      unit is present, then disables the battery backup unit on the specified
 *      controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The battery backup unit for the
 *                              specified controller is disabled.
 *      TW_RET_FAILED       - The battery backup unit for the
 *                              specified controller is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable a particular controller's battery backup
 *      unit.  A Disabled BBU will not be used by the controller in the event
 *      of a power failure.
 *
 *    See also:
 *      tw_bbu_enable
 *      tw_get_bbu_online_state
 *      tw_get_bbu_status_summary
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_disable ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 & 9690 only)
 *
 *    Description:
 *      Stops capacity test of the battery on the battery backup unit.
 *      The API verifies that the controller ID is valid, and the
 *      battery backup unit is present, then tests the battery.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successful in stoping the battery capacity test
 *                              for the specified controller
 *      TW_RET_FAILED       - Failed to stop the battery capacity test for
 *                              the specified controller
 *
 *    Remarks:
 *      Use this routine to stop the capacity test of the battery on the battery backup unit.
 *      In order to start a new capacity test or a high current capacity test (a short test),
 *      First need to stop the current capacity test in progress.
 *
 *    See also:
 *      tw_bbu_test_battery
 *      tw_bbu_initialize_battery
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_stop_battery_test ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 & 9690 only)
 *
 *    Description:
 *      Does high-current capacity test of the battery on the battery backup unit.
 *      The API verifies that the controller ID is valid, and the
 *      battery backup unit is present, then tests the battery.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successful in starting the high-current capacity test of the battery
 *                              for the specified controller.
 *      TW_RET_FAILED       - Failed to start the high-current capacity test of the battery
 *                              for the specified controller. See tw_get_error_information() for error details.
 *
 *    Remarks:
 *      Use this routine to start the high-current battery capacity test.
 *      The battery should be tested periodically to measure the effective
 *      backup time it can provide.  This is a short test takes few seconds to
 *      complete.  During this test the data protection feature provided
 *      by the battery backup unit is disabled.
 *
 *    See also:
 *      tw_bbu_stop_battery_test
 *      tw_bbu_test_battery
 *      tw_bbu_initialize_battery
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_battery_high_current_test ( tw_controller_id_type   controller_id );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *                                          Nvram controller
 *
 *    Description:
 *      Updates firmware on the battery backup unit with the BBU firmware image that was
 *      downloaded to a controller. The API verifies that the controller ID is valid,
 *      and the battery backup unit is present, then initiates BBU firmware update.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The bbu firmware update for the specified controller
 *                              is being completed.
 *      TW_RET_FAILED       - Failed to update bbu firmware for the specified controller.
 *                              see tw_get_error_information() to get details on error.
 *
 *    Remarks:
 *      Use this routine to update firmware on the battery backup unit.
 *      Before calling this routine, the battery backup unit must be installed and
 *      the BBU firmware image downloaded to a controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_bbu_firmware_update ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Sets the phy link negotiation limit on the specified phy.
 *      The API verifies that the phy ID is valid, then stores the control
 *      setting of the specified phy on the phy's controller.
 *
 *    Parameters:
 *      phy_id          - ID of the phy in question.
 *      link_limit      - The value tw_set_phy_link_negotiation_limit
 *                          will use to set the phy link speed negotiation limit 
 *                          Must be one of:
 *                            TW_PHY_LINK_SPEED_AUTO
 *                            TW_PHY_LINK_SPEED_LIMIT_1_5_GBPS
 *                            TW_PHY_LINK_SPEED_LIMIT_3_0_GBPS
 *                            TW_PHY_LINK_SPEED_LIMIT_6_0_GBPS
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - Succeeded in storing the control setting of the specified phy.
 *                              
 *      TW_RET_FAILED   - Failed to store the control setting of the specified phy.
 *                          see tw_get_error_information for details.
 *
 *    Remarks:
 *      Use this routine to set a particular phy's link speed negotiation limit.
 *
 *    See also:
 *      tw_get_phy_link_negotiation_limit
 *      tw_get_phy_link_capability
 *      tw_get_phy_link_status
 *      tw_get_phy_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_phy_link_negotiation_limit (
      tw_phy_id_type      phy_id,
      tw_phy_link_type    link_limit
                          );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then identifies
 *      the specified enclosure.
 *
 *    Parameters:
 *      enclosure_id       - ID of the enclosure of interest.
 *      on_off             - Identify state the enclosure should assume.
 *                              Must be one of:
 *                                TW_ON  - Identify enclosure by a visual indication.
 *                                TW_OFF - Do not identify enclosure.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successful in changing the identify state of the specified enclosure.
 *      TW_RET_FAILED       - Failure occurred. See tw_get_error_information() for error details.
 *
 *    Remarks:
 *      Use this routine to change the identify state of a particular enclosure.
 *
 *    See also:
 *      tw_get_enclosure_identify_status
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_enclosure (
      tw_enclosure_id_type     enclosure_id,
      tw_identify_status_type  on_off
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      This routine examines a firmware image, an enclosure and provides a
 *      recommendation for Firmware Update. The caller is free to follow the
 *      recommendation or ignore it and proceed with enclosure firmware download
 * 
 *    Parameters:
 *      enclosure_id         - ID of the enclosure in question.
 *      sep_number           - SEP number, some enclosures have two or more seperate expander backplane
 *                             and each backplane has its own firmware flash region to be updated.
 *      prom_name            - Firmware image file name.
 *      compatibility        - Address of compatibility.
 *                               Defined as:
 *                                 typedef  struct _tw_enclosure_compatibility_tag
 *                                 {
 *                                   unsigned char           comp;
 *                                   char                    curr_ver[TW_MAX_FIRMWARE_VERSION_LEN];
 *                                   char                    new_ver[TW_MAX_FIRMWARE_VERSION_LEN];
 *                                 } tw_enclosure_compatibility_type;
 *                            
 *                                "comp" field will be set to one of these values,
 *                                  - TW_DOWNLOAD_MICROCODE_NOT_COMPATIBLE
 *                                      Firmware image is NOT compatible with the enclosure.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_NEW
 *                                      Firmware image is newer than the current enclosure firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_OLD
 *                                      Firmware image is older than the current enclosure firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_SAME
 *                                      Firmware image is of the same version as the current enclosure firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_NOT_KNOWN
 *                                      Firmware image version or the current enclosure firmware version not known.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified compatiblity contains a recommendation
 *                              for the download of specified firmware image.
 *      TW_RET_FAILED       - Failed to check compatiblity, see tw_get_error_information() for more details.
 *
 *
 *    See also:
 *      tw_enclosure_firmware_download()
 *      tw_get_enclosure_fw_version()
 *      tw_get_enclosure_id_list()
 * </pre>
 *
 */

int tw_check_enclosure_image_compatibility (
      tw_enclosure_id_type                enclosure_id,
      unsigned char                       sep_number,
      char*                               prom_name,
      tw_enclosure_compatibility_type    *compatibility
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      This routine downloads image file to enclosure. After the successful download
 *      Reset/Reboot the enclosure for the downloaded firmware to be active.
 * 
 *    Parameters:
 *      enclosure_id         - ID of the enclosure in question.
 *      sep_number           - SEP number, some enclosures have two or more seperate expander backplane
 *                             and each backplane has its own firmware flash region to be updated.
 *      prom_name            - Firmware image file name.
 *      force                - Skip any validation if is a non-zero value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified firmware image was successfully downloaded.
 *      TW_RET_FAILED       - The specified firmware image was not successfully downloaded.
 *                              see tw_get_error_information() for more details.
 *
 *
 *    See also:
 *      tw_check_enclosure_image_compatibility()
 *      tw_get_enclosure_fw_version()
 *      tw_get_enclosure_id_list()
 * </pre>
 *
 */

int tw_enclosure_firmware_download (
      tw_enclosure_id_type   enclosure_id,
      unsigned char          sep_number,
      char*                  prom_name,
      int                    force
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified fan.
 *      The API verifies that the fan ID is valid, then identifies
 *      the specified fan.
 *
 *    Parameters:
 *      fan_id       - ID of the fan of interest.
 *      on_off              - State the fan should assume.
 *                              Must be one of:
 *                                TW_ON  - fan is identified.
 *                                TW_OFF - fan is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified fan is identified.
 *      TW_RET_FAILED       - The specified fan is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular fan.
 *
 *    See also:
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_fan (
      tw_fan_id_type           fan_id,
      tw_identify_status_type  on_off
                    );


/**
 * <pre>
 *    Supported enclosures:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Sets the actual speed or rate of cooling of the specified fan.
 *      The API verifies that the fan ID is valid, then sends the requests to specified
 *      fan's enclosure.
 *
 *    Parameters:
 *      fan_id     - ID of the fan in question.
 *      fan_speed  - The address where tw_set_fan_speed will store the fan speed,
 *                     with any of the following values:
 *                       TW_ENCL_FAN_SPEED_LOWEST
 *                         This indicates that the fan is running at lowest speed.
 *                       TW_ENCL_FAN_SPEED_SECOND_LOWEST
 *                         This indicates that the fan is running at second lowest speed.
 *                       TW_ENCL_FAN_SPEED_THIRD_LOWEST
 *                         This indicates that the fan is running at third lowest speed.
 *                       TW_ENCL_FAN_SPEED_INTERMEDIATE
 *                         This indicates that the fan is running at intermediate speed.
 *                       TW_ENCL_FAN_SPEED_THIRD_HIGHEST
 *                         This indicates that the fan is running at third highest speed.
 *                       TW_ENCL_FAN_SPEED_SECOND_HIGHEST
 *                         This indicates that the fan is running at second highest speed.
 *                       TW_ENCL_FAN_SPEED_HIGHEST
 *                         This indicates that the fan is running at highest speed.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - Successfully set the fan speed for the specified fan.
 *                           
 *      TW_RET_FAILED   - failure occurred in setting the fan speed for the specified fan.
 *
 *    Remarks:
 *      Applications can use this routine to set the fan speed or rate of cooling
 *      of a particular fan.
 *
 *    See also:
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
*/
int tw_set_fan_speed (
      tw_fan_id_type        fan_id,
      tw_fan_speed_type     fan_speed
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Enables the specified fan.
 *      The API verifies that the fan ID is valid, then enables the
 *      specified fan.
 *
 *    Parameters:
 *      fan_id              - ID of the fan in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified fan is enabled.
 *      TW_RET_FAILED       - The specified fan state is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable a particular fan.
 *
 *    See also:
 *      tw_get_fan_status
 *      tw_enable_fan
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_fan ( tw_fan_id_type   fan_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Disables the specified fan.
 *      The API verifies that the fan ID is valid, then disables the
 *      specified fan.
 *
 *    Parameters:
 *      fan_id              - ID of the fan in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified fan is disabled.
 *      TW_RET_FAILED       - The specified fan state is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable a particular fan.
 *
 *    See also:
 *      tw_get_fan_status
 *      tw_disable_fan
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_fan ( tw_fan_id_type   fan_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified drive slot.
 *      The API verifies that the drive slot ID is valid, then identifies
 *      the specified drive slot.
 *
 *    Parameters:
 *      drive_slot_id       - ID of the drive slot of interest.
 *      on_off              - State the drive slot should assume.
 *                              Must be one of:
 *                                TW_ON  - drive slot is identified.
 *                                TW_OFF - drive slot is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified drive slot is identified.
 *      TW_RET_FAILED       - The specified drive slot is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular drive slot.
 *
 *    See also:
 *      tw_get_drive_slot_identify_status
 *      tw_get_port_identify_status
 *      tw_identify_port
 *      tw_get_unit_identify_status
 *      tw_identify_unit
 *      tw_get_drive_slot_status
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_drive_slot (
      tw_drive_slot_id_type     drive_slot_id,
      tw_identify_status_type   on_off
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified temperature sensor.
 *      The API verifies that the temperature sensor ID is valid, then identifies
 *      the specified temperature sensor.
 *
 *    Parameters:
 *      temp_sensor_id      - ID of the temperature sensor of interest.
 *      on_off              - State the temperature sensor should assume.
 *                              Must be one of:
 *                                TW_ON  - temperature sensor is identified.
 *                                TW_OFF - temperature sensor is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified temperature sensor is identified.
 *      TW_RET_FAILED       - The specified temperature sensor is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular temperature sensor.
 *
 *    See also:
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_temp_sensor (
      tw_temp_sensor_id_type    temp_sensor_id,
      tw_identify_status_type   on_off
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified power supply.
 *      The API verifies that the power supply ID is valid, then identifies
 *      the specified power supply.
 *
 *    Parameters:
 *      power_supply_id      - ID of the power supply of interest.
 *      on_off              - State the power supply should assume.
 *                              Must be one of:
 *                                TW_ON  - power supply is identified.
 *                                TW_OFF - power supply is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified power supply is identified.
 *      TW_RET_FAILED       - The specified power supply is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular power supply.
 *
 *    See also:
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_power_supply (
      tw_power_supply_id_type    power_supply_id,
      tw_identify_status_type    on_off
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Enables the specified power supply.
 *      The API verifies that the power supply ID is valid, then enables the
 *      specified power supply.
 *
 *    Parameters:
 *      power_supply_id     - ID of the power supply in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified power supply is enabled.
 *      TW_RET_FAILED       - The specified power supply state is unchanged.
 *
 *    Remarks:
 *      Use this routine to enable a particular power supply.
 *
 *    See also:
 *      tw_get_power_supply_status
 *      tw_enable_power_supply
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_power_supply ( tw_power_supply_id_type   power_supply_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Disables the specified power supply.
 *      The API verifies that the power supply ID is valid, then disables the
 *      specified power supply.
 *
 *    Parameters:
 *      power_supply_id     - ID of the power supply in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified power supply is disabled.
 *      TW_RET_FAILED       - The specified power supply state is unchanged.
 *
 *    Remarks:
 *      Use this routine to disable a particular power supply.
 *
 *    See also:
 *      tw_get_power_supply_status
 *      tw_disable_power_supply
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_power_supply ( tw_power_supply_id_type   power_supply_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified alarm.
 *      The API verifies that the alarm ID is valid, then identifies
 *      the specified alarm.
 *
 *    Parameters:
 *      alarm_id       - ID of the alarm of interest.
 *      on_off         - State the alarm should assume.
 *                         Must be one of:
 *                           TW_ON  - alarm is identified.
 *                           TW_OFF - alarm is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified alarm is identified.
 *      TW_RET_FAILED       - The specified alarm is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular alarm.
 *
 *    See also:
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_alarm (
      tw_alarm_id_type         alarm_id,
      tw_identify_status_type  on_off
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Mutes the specified alarm.
 *      The API verifies that the alarm ID is valid, then mutes the
 *      specified alarm. When the alarm is in the muted state, it will not emit sound
 *      regardless of the error condition that exists. When the alarm is un-muted,
 *      it may emit sound if an error condition exist.
 *
 *    Parameters:
 *      alarm_id            - ID of the alarm in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified alarm is muted.
 *      TW_RET_FAILED       - The specified alarm state is unchanged.
 *
 *    Remarks:
 *      Use this routine to mute a particular alarm.
 *
 *    See also:
 *      tw_unmute_alarm
 *      tw_get_alarm_status
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_mute_alarm ( tw_alarm_id_type   alarm_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Un-mutes the specified alarm.
 *      The API verifies that the alarm ID is valid, then un-mutes the
 *      specified alarm. When the alarm is in the muted state, it will not emit sound
 *      regardless of the error condition that exists. When the alarm is un-muted,
 *      it may emit sound if an error condition exist.
 *
 *    Parameters:
 *      alarm_id            - ID of the alarm in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified alarm is un-muted.
 *      TW_RET_FAILED       - The specified alarm state is unchanged.
 *
 *    Remarks:
 *      Use this routine to un-mute a particular alarm.
 *
 *    See also:
 *      tw_get_alarm_status
 *      tw_mute_alarm
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unmute_alarm ( tw_alarm_id_type   alarm_id );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Requests the specified alarm to emit tone.
 *
 *    Parameters:
 *      alarm_id            - ID of the alarm in question.
 *      alram_tone          - Tone urgency,
 *                            The value must be one of the following:
 *                              TW_ALARM_TONE_URGENCY_INFO
 *                                Specifies that the audible alarm emit a tone suitable
 *                                to warn of an information condition.
 *                              TW_ALARM_TONE_URGENCY_NON_CRITICAL
 *                                Specifies that the audible alarm emit a tone suitable
 *                                to warn of a non-critical condition.
 *                              TW_ALARM_TONE_URGENCY_CRITICAL
 *                                Specifies that the audible alarm emit a tone suitable
 *                                to warn of a critical condition.
 *                              TW_ALARM_TONE_URGENCY_UNRECOVERABLE
 *                                Specifies that the audible alarm emit a tone suitable
 *                                to warn of a unrecoverable condition.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified alarm is enabled.
 *      TW_RET_FAILED       - The specified alarm state is unchanged.
 *
 *    Remarks:
 *      Use this routine to set an audible tone of a particular alarm. The quality of each tone
 *      and the implementation of seperate tones is enclosure vender specific. If more than one
 *      tone is set, then the tone that signals the most urgent of the selected condition is activated.
 *
 *    See also:
 *      tw_set_alarm_off
 *      tw_get_alarm_status
 *      tw_mute_alarm
 *      tw_unmute_alarm
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_alarm_on (
      tw_alarm_id_type   alarm_id,
      tw_alarm_tone_type alarm_tone
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Requests the specified alarm to stop emitting tone.
 *
 *    Parameters:
 *      alarm_id            - ID of the alarm in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified alarm is enabled.
 *      TW_RET_FAILED       - The specified alarm state is unchanged.
 *
 *    Remarks:
 *      Use this routine to stop an audible tone of a particular alarm.
 *
 *    See also:
 *      tw_set_alarm_on
 *      tw_get_alarm_status
 *      tw_mute_alarm
 *      tw_unmute_alarm
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_alarm_off ( tw_alarm_id_type   alarm_id );


#ifndef   Example
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified door lock.
 *      The API verifies that the door lock ID is valid, then identifies
 *      the specified door lock.
 *
 *    Parameters:
 *      door_lock_id       - ID of the door lock of interest.
 *      on_off              - State the door lock should assume.
 *                              Must be one of:
 *                                TW_ON  - door lock is identified.
 *                                TW_OFF - door lock is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified door lock is identified.
 *      TW_RET_FAILED       - The specified door lock is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular door lock.
 *
 *    See also:
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_door_lock (
      tw_door_lock_id_type     door_lock_id,
      tw_identify_status_type  on_off
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Locks the specified door.
 *      The API verifies that the door lock ID is valid, then locks the
 *      specified door.
 *
 *    Parameters:
 *      door_lock_id        - ID of the door lock in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified door is locked.
 *      TW_RET_FAILED       - The specified door state is unchanged.
 *
 *    Remarks:
 *      Use this routine to lock a particular door.
 *
 *    See also:
 *      tw_get_door_lock_status
 *      tw_unlock_door
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_lock_door ( tw_door_lock_id_type   door_lock_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Unlocks the specified door.
 *      The API verifies that the door lock ID is valid, then unlocks the
 *      specified door.
 *
 *    Parameters:
 *      door_lock_id        - ID of the door lock in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified door is unlocked.
 *      TW_RET_FAILED       - The specified door state is unchanged.
 *
 *    Remarks:
 *      Use this routine to unlock a particular door.
 *
 *    See also:
 *      tw_get_door_lock_status
 *      tw_lock_door
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unlock_door ( tw_door_lock_id_type   door_lock_id );


/**
 * <pre>
 *  Asynchronous Events
 *      3ware controllers log events asynchronously to the host.
 *  This API provides an interface for the applications to create and log software events.
 *
 *
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates and add software event to controller's asynchronous event queue.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      event_info        - The address where tw_create_event will
 *                            will store the event information.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Successful in adding an event to the controller event queue.
 *      TW_RET_FAILED     - Failed to add an event to the controller event queue. 
 *                            See tw_get_error_information() to get error details.
 *
 *    Remarks:
 *      Use this routine to add software events to a particular controller's
 *      asynchronous event queue. Software events such as state changes in enclosure
 *      element detected by the enclosure monitor. The added software event will be 
 *      reported/logged to Host through the existing asynchronous events reporting mechanism. 
 *      Refer to "See Also" section below for more information.
 *
 *      The event information is passed in a structure of type tw_create_event_type.
 *      The following fields within the structure are of interest:
 *        aen_code    - A 16-bit hex value indicating the specific AEN.
 *        severity    - Specify severity level of AEN,
 *                        The value must be one of the following:
 *                          TW_AEN_SEVERITY_ERROR
 *                          TW_AEN_SEVERITY_WARNING
 *                          TW_AEN_SEVERITY_INFORMATION
 *                          TW_AEN_SEVERITY_DEBUG
 *        source      - Specify application that is requesting to add AEN,
 *                        The value must be one of the following:
 *                          TW_AEN_SOURCE_3DM
 *                          TW_AEN_SOURCE_ENCLOSURE_MONITOR
 *                          TW_AEN_SOURCE_CLI
 *                          TW_AEN_SOURCE_SCOUT
 *                          TW_AEN_SOURCE_3RD_PARTY
 *        reserved_0  - Reserved for future use.
 *        reserved_1  - Reserved for future use.
 *        param_data  - Short or a parameter string in ASCII, NULL terminated.
 *                        for example: "enclosure=0, logical ID=0x50A1B2C3D4E5F6G7 fan=2".
 *        param_len   - Length of the string 'param_data' field NOT including
 *                        the terminating NULL character.
 *        desc        - Description of AEN in ASCII, NULL terminated.
 *                        for example: "Enclosure fan failed".
 *        desc_len    - Length of the string in 'desc' field NOT including
 *                        the NULL character.
 *        sense_data  - 18-bytes scsi sense data.
 *
 *    See also:
 *      tw_get_first_event
 *      tw_get_last_event
 *      tw_get_next_event
 *      tw_get_previous_event
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_event (
      tw_controller_id_type   controller_id,
      tw_create_event_type*   event_info
       );


#endif /* Example */


/*******************************************************************************
 * Unit Configuration and Management Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Creates a unit with the specified drives.  The API verifies that the
 *      drive list, unit configuration and stripe size are valid, then creates
 *      a unit with the specified drives, the specified configuration and the
 *      specified stripe size.  The unit ID of the newly created unit is
 *      written to the location specified by the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_CBOD
 *                                TW_UNIT_CONFIG_IBOD
 *                                TW_UNIT_CONFIG_JBOD
 *                                TW_UNIT_CONFIG_SPARE
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The stripe size tw_create_unit will specify
 *                              will store the stripe size (in K bytes).
 *                              Stripe size is supported for the following
 *                              unit configurations:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6  (supports 64K & 256K stripe sizes only)
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *                              The following stripe sizes are only supported on 7000 & 8000 series.
 *                                TW_UNIT_STRIPE_SIZE_8K
 *                                TW_UNIT_STRIPE_SIZE_32K
 *                                TW_UNIT_STRIPE_SIZE_128K
 *                                TW_UNIT_STRIPE_SIZE_512K
 *                                TW_UNIT_STRIPE_SIZE_1024K
 *
 *                              For all other unit configurations use
 *                                TW_UNIT_STRIPE_SIZE_UNUSED
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      Use this routine to create a new unit, using available drives, on a
 *      particular controller.  This routine will initialize drive media to
 *      prepare it for use.  Any data present on the drives will no longer
 *      be accessible once the unit is created.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id
                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates a RAID 50 unit with the specified drives and layout.
 *      The API verifies that the drive list, stripe size, and layout are
 *      valid, then creates a RAID 50 unit with the specified drives,
 *      stripe size and layout.  The unit ID of the newly created unit is
 *      written to the location specified by the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be:
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The following stripe size (in K bytes)
 *                              supported for the RAID 50 configuration and
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *      layout              - The number of disks per disk-group.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_RAID_50_G3
 *                                TW_UNIT_CONFIG_RAID_50_G4
 *                                TW_UNIT_CONFIG_RAID_50_G5
 *                                TW_UNIT_CONFIG_RAID_50_G6
 *                                TW_UNIT_CONFIG_RAID_50_G7
 *                                TW_UNIT_CONFIG_RAID_50_G8
 *                                TW_UNIT_CONFIG_RAID_50_G9
 *                                TW_UNIT_CONFIG_RAID_50_G10
 *                                TW_UNIT_CONFIG_RAID_50_G11
 *                                TW_UNIT_CONFIG_RAID_50_G12
 *                                TW_UNIT_CONFIG_RAID_50_G13
 *                                TW_UNIT_CONFIG_RAID_50_G14
 *                                TW_UNIT_CONFIG_RAID_50_G15
 *                                TW_UNIT_CONFIG_RAID_50_G16
 *                              For all other unit configurations use
 *                                TW_UNIT_CONFIG_RAID_50_G_UNUSED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      Use this routine to create a new unit, using available drives, on a
 *      particular controller with a specific layout of 3, 4, 5, 6, 7, 8, 9,
 *      10, 11, 12, 13, 14, 15, or 16 drives in each RAID 5 group.
 *      This routine will initialize drive media to prepare it for use. Any data
 *      present on the drives will no longer be accessible once the unit is created.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_with_layout (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id,
      unsigned int             layout
                               ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates a unit with the specified drives.  The API verifies that the
 *      drive list, unit configuration and stripe size are valid, then creates
 *      a unit with the specified drives, the specified configuration and the
 *      specified stripe size. In addition, allows to create boot lun (LUN 0) of the size specified.
 *      The unit ID of the newly created unit is written to the location specified
 *      by the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit_2 will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_CBOD
 *                                TW_UNIT_CONFIG_IBOD
 *                                TW_UNIT_CONFIG_JBOD
 *                                TW_UNIT_CONFIG_SPARE
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The stripe size tw_create_unit will specify
 *                              will store the stripe size (in K bytes).
 *                              Stripe size is supported for the following
 *                              unit configurations:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6  (supports 64K & 256K stripe sizes only)
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *                              The following stripe sizes are only supported on 7000 & 8000 series.
 *                                TW_UNIT_STRIPE_SIZE_8K
 *                                TW_UNIT_STRIPE_SIZE_32K
 *                                TW_UNIT_STRIPE_SIZE_128K
 *                                TW_UNIT_STRIPE_SIZE_512K
 *                                TW_UNIT_STRIPE_SIZE_1024K
 *
 *                              For all other unit configurations use
 *                                TW_UNIT_STRIPE_SIZE_UNUSED
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *      lun0_size           - The size of the boot lun or lun 0(in KBytes)
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      This is an extension of the tw_create_unit() API. It creates boot lun (LUN 0)
 *      during unit creation. Use this routine to create a new unit, using available drives,
 *      on a particular controller.  This routine will initialize drive media to
 *      prepare it for use.  Any data present on the drives will no longer
 *      be accessible once the unit is created.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_2 (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id,
      tw_lun_size_type         lun0_size
                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates a RAID 50 unit with the specified drives and layout.
 *      The API verifies that the drive list, stripe size, and layout are
 *      valid, then creates a RAID 50 unit with the specified drives,
 *      stripe size and layout. In addition, allows to create boot lun (LUN 0)
 *      of the size specified.  The unit ID of the newly created unit is
 *      written to the location specified by the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit_with_layout_2 will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be:
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The following stripe size (in K bytes)
 *                              supported for the RAID 50 configuration and
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *      layout              - The number of disks per disk-group.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_RAID_50_G3
 *                                TW_UNIT_CONFIG_RAID_50_G4
 *                                TW_UNIT_CONFIG_RAID_50_G5
 *                                TW_UNIT_CONFIG_RAID_50_G6
 *                                TW_UNIT_CONFIG_RAID_50_G7
 *                                TW_UNIT_CONFIG_RAID_50_G8
 *                                TW_UNIT_CONFIG_RAID_50_G9
 *                                TW_UNIT_CONFIG_RAID_50_G10
 *                                TW_UNIT_CONFIG_RAID_50_G11
 *                                TW_UNIT_CONFIG_RAID_50_G12
 *                                TW_UNIT_CONFIG_RAID_50_G13
 *                                TW_UNIT_CONFIG_RAID_50_G14
 *                                TW_UNIT_CONFIG_RAID_50_G15
 *                                TW_UNIT_CONFIG_RAID_50_G16
 *                              For all other unit configurations use
 *                                TW_UNIT_CONFIG_RAID_50_G_UNUSED
 *      lun0_size           - The size of the boot lun or lun 0(in KBytes)
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      This is an extension of the tw_create_unit_with_layout() API.
 *      It creates boot lun (LUN 0) during unit creation.
 *      Use this routine to create a new unit, using available drives, on a
 *      particular controller with a specific layout of 3, 4, 5, 6, 7, 8, 9,
 *      10, 11, 12, 13, 14, 15, or 16 drives in each RAID 5 group.
 *      This routine will initialize drive media to prepare it for use. Any data
 *      present on the drives will no longer be accessible once the unit is created.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_with_layout_2 (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id,
      unsigned int             layout,
      tw_lun_size_type         lun0_size             
                               ); 



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates a unit with the specified drives.  The API verifies that the
 *      drive list, unit configuration and stripe size are valid, then creates
 *      a unit with the specified drives, the specified configuration and the
 *      specified stripe size. In addition, allows to create LUNs of user defined sizes.
 *      The unit ID of the newly created unit is written to the location specified
 *      by the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit_3 will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_CBOD
 *                                TW_UNIT_CONFIG_IBOD
 *                                TW_UNIT_CONFIG_JBOD
 *                                TW_UNIT_CONFIG_SPARE
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The stripe size tw_create_unit will specify
 *                              will store the stripe size (in K bytes).
 *                              Stripe size is supported for the following
 *                              unit configurations:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6  (supports 64K & 256K stripe sizes only)
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *                              The following stripe sizes are only supported on 7000 & 8000 series.
 *                                TW_UNIT_STRIPE_SIZE_8K
 *                                TW_UNIT_STRIPE_SIZE_32K
 *                                TW_UNIT_STRIPE_SIZE_128K
 *                                TW_UNIT_STRIPE_SIZE_512K
 *                                TW_UNIT_STRIPE_SIZE_1024K
 *
 *                              For all other unit configurations use
 *                                TW_UNIT_STRIPE_SIZE_UNUSED
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *      lun_sizes           - An array of user defined LUN sizes starting with boot lun or LUN zero(in KBytes)
 *      num_of_lun_sizes    - A number of user defined LUN sizes in lun_sizes array.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      This is an extension of the tw_create_unit() API. It creates LUNs
 *      with user-specified LUN sizes during unit creation.
 *      Use this routine to create a new unit, using available drives,
 *      on a particular controller.  This routine will initialize drive media to
 *      prepare it for use.  Any data present on the drives will no longer
 *      be accessible once the unit is created.
 *
 *      NOTE: The API will first create LUNs of the size specified by the caller, then 
 *      the rest of the unit is carved with auto LUN carving.
 *      For example: let us say caller of the API asked to create a 10 TB unit and specified
 *      lun_sizes[0] = 500GB, lun_sizes[1] = 1 TB. The auto carve LUN size is set to 2 TB.
 *      The result of the API call will be,
 *          LUN 0 = 500GB
 *          LUN 1 = 1 TB
 *          LUN 2 = 2 TB
 *          LUN 3 = 2 TB
 *          LUN 4 = 2 TB
 *          LUN 5 = 2 TB
 *          LUN 6 = .5 TB
 *
 *    See also:
 *      tw_create_unit()
 *      tw_create_unit_2()
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_3 (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id,
      tw_lun_size_type*        lun_sizes,
      int                      num_of_lun_sizes   
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Creates a RAID 50 unit with the specified drives and layout.
 *      The API verifies that the drive list, stripe size, and layout are
 *      valid, then creates a RAID 50 unit with the specified drives,
 *      stripe size and layout. In addition, allows to create LUNs of user defined sizes.
 *      The unit ID of the newly created unit is written to the location specified by
 *      the unit_id parameter.
 *
 *    Parameters:
 *      drive_id_list       - The available drives tw_create_unit_with_layout_3 will
 *                              use to create the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be:
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The following stripe size (in K bytes)
 *                              supported for the RAID 50 configuration and
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      unit_id             - The address of the newly created unit ID.
 *      layout              - The number of disks per disk-group.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_RAID_50_G3
 *                                TW_UNIT_CONFIG_RAID_50_G4
 *                                TW_UNIT_CONFIG_RAID_50_G5
 *                                TW_UNIT_CONFIG_RAID_50_G6
 *                                TW_UNIT_CONFIG_RAID_50_G7
 *                                TW_UNIT_CONFIG_RAID_50_G8
 *                                TW_UNIT_CONFIG_RAID_50_G9
 *                                TW_UNIT_CONFIG_RAID_50_G10
 *                                TW_UNIT_CONFIG_RAID_50_G11
 *                                TW_UNIT_CONFIG_RAID_50_G12
 *                                TW_UNIT_CONFIG_RAID_50_G13
 *                                TW_UNIT_CONFIG_RAID_50_G14
 *                                TW_UNIT_CONFIG_RAID_50_G15
 *                                TW_UNIT_CONFIG_RAID_50_G16
 *                              For all other unit configurations use
 *                                TW_UNIT_CONFIG_RAID_50_G_UNUSED
 *      lun_sizes           - An array of user defined LUN sizes starting with boot lun or LUN zero(in KBytes)
 *      num_of_lun_sizes    - A number of user defined LUN sizes in lun_sizes array.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The new unit with the specified stripe size and
 *                              with the specified configuration is available.
 *      TW_RET_FAILED       - The new unit is NOT available.
 *
 *    Remarks:
 *      This is an extension of the tw_create_unit_with_layout() API. It creates LUNs
 *      with user-specified LUN sizes during unit creation.
 *      Use this routine to create a new unit, using available drives, on a
 *      particular controller with a specific layout of 3, 4, 5, 6, 7, 8, 9,
 *      10, 11, 12, 13, 14, 15, or 16 drives in each RAID 5 group.
 *      This routine will initialize drive media to prepare it for use. Any data
 *      present on the drives will no longer be accessible once the unit is created.
 *
 *      NOTE: The API will first create LUNs of the size specified by the caller, then 
 *      the rest of the unit is carved with auto LUN carving.
 *      For example: let us say caller of the API asked to create a 10 TB unit and specified
 *      lun_sizes[0] = 500GB, lun_sizes[1] = 1 TB. The auto carve LUN size is set to 2 TB.
 *      The result of the API call will be,
 *          LUN 0 = 500GB
 *          LUN 1 = 1 TB
 *          LUN 2 = 2 TB
 *          LUN 3 = 2 TB
 *          LUN 4 = 2 TB
 *          LUN 5 = 2 TB
 *          LUN 6 = .5 TB
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_with_layout_3 (
      tw_drive_id_list_type*   drive_id_list,
      tw_configuration_type    unit_config,
      tw_stripe_size_type      stripe_size,
      unsigned char            make_os_aware,
      tw_unit_id_type*         unit_id,
      unsigned int             layout,
      tw_lun_size_type*        lun_sizes,
      int                      num_of_lun_sizes 
                                 ); 



/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Destroys the specified unit.  The API verifies that the unit ID
 *      is valid, then destroys the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be destroyed.
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the destroyed unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_DESTROYED_UNIT
 *                                    TW_NOTIFY_OS_OF_DESTROYED_UNIT
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is destroyed.
 *      TW_RET_FAILED       - The specified unit is NOT destroyed.
 *
 *    Remarks:
 *      Use this routine to destroy existing units on a particular controller.
 *      This routine will overwrite drive data.  Any data present on the
 *      drives will no longer be accessible once the unit is destroyed.
 *      Use tw_remove_drive before physically detaching drives from the
 *      controller.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_remove_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_destroy_unit (
      tw_unit_id_type   unit_id,
      unsigned char     make_os_aware
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Imports a unit with the specified drives.  The API verifies that the
 *      drive list is valid, then imports a previously created unit using the
 *      specified drives.  The unit IDs of any newly imported unit are written
 *      to the location specified by the unit_id_list parameter.
 *
 *    Parameters:
 *      drive_id_list       - The drives tw_import_unit will use
 *                              to import the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the imported unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_IMPORTED_UNIT
 *                                    TW_NOTIFY_OS_OF_IMPORTED_UNIT
 *      unit_id_list        - The address of a list of newly imported unit IDs.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The imported unit is available.
 *      TW_RET_FAILED       - The imported unit is NOT available.
 *
 *    Remarks:
 *      Use this routine to import a previously created unit, using available
 *      drives, on a particular controller.  This routine will use the existing
 *      drive data to prepare it for use.  Any data present on the drives will
 *      be accessible once the unit is imported.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_import_unit (
      tw_drive_id_list_type*   drive_id_list,
      unsigned char            make_os_aware,
      tw_unit_id_list_type*    unit_id_list
                   ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Exports the specified unit.  The API verifies that the unit ID
 *      is valid, then exports the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be exported.
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the exported unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_EXPORTED_UNIT
 *                                    TW_NOTIFY_OS_OF_EXPORTED_UNIT
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is exported.
 *      TW_RET_FAILED       - The specified unit is NOT exported.
 *
 *    Remarks:
 *      Use this routine to export existing units on a particular controller.
 *      This routine will leave drive data intact.  Any data present on the
 *      drives can be accessed after the unit is imported with tw_import_unit.
 *      Use tw_remove_drive before physically detaching drives from the
 *      controller.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_remove_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_export_unit (
      tw_unit_id_type   unit_id,
      unsigned char     make_os_aware
                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Exports a unit, then removes all drives associated with the unit.
 *      The API verifies that the unit ID is valid, exports the specified unit
 *      and removes all drives associated with the unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be removed.
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the exported unit.
 *                              Must be one of:
 *                                TW_DO_NOT_NOTIFY_OS_OF_EXPORTED_UNIT
 *                                TW_NOTIFY_OS_OF_EXPORTED_UNIT
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is removed.
 *      TW_RET_FAILED       - The specified unit is NOT removed.
 *
 *    Remarks:
 *      Use this routine if you want to remove the drives from the controller
 *      in addition to exporting the unit.  This could easily accomplished
 *      by calling tw_export_unit on a unit, then subsequently calling
 *      tw_remove_drive on each of the drives that made up the unit.
 *      This routine just makes it easier.  Also for single disk units
 *      (e.g. JBOD, IBOD, SBOD) this routine should be called instead of
 *      tw_remove_drive to ensure that the unit information is removed from
 *      the controller.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_export_unit
 *      tw_remove_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_remove_unit (
      tw_unit_id_type   unit_id,
      unsigned char     make_os_aware
                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Migrates the specified unit from its current configuration to the
 *      specified configuration using the unit's drives, additional drives,
 *      as specified, and the specified stripe size and drive layout.
 *      The API verifies that the unit ID, unit configuration, drive list,
 *      stripe size, and layout are valid, then migrates the specified unit.
 *      The unit ID of the migrated unit is the same as the original unit ID.
 *
 *    Parameters:
 *      unit_id             - The ID of the unit to migrate.
 *                              This unit's configuration must be one of:
 *                                TW_UNIT_CONFIG_IBOD
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *      drive_id_list       - The drives tw_migrate_unit will use in
 *                              the new unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      unit_config         - The configuration of the new unit.
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_TWINSTOR
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *      stripe_size         - The stripe size tw_migrate_unit will specify
 *                              will store the stripe size (in K bytes).
 *                              Stripe size is supported for the following
 *                              unit configurations:
 *                                TW_UNIT_CONFIG_RAID_0
 *                                TW_UNIT_CONFIG_RAID_5
 *                                TW_UNIT_CONFIG_RAID_6   (supports 64K & 256K stripe sizes only)
 *                                TW_UNIT_CONFIG_RAID_10
 *                                TW_UNIT_CONFIG_RAID_50
 *                              Must be one of:
 *                                TW_UNIT_STRIPE_SIZE_16K
 *                                TW_UNIT_STRIPE_SIZE_64K
 *                                TW_UNIT_STRIPE_SIZE_256K
 *                              The following stripe sizes are only supported on 7000 & 8000 series.
 *                                TW_UNIT_STRIPE_SIZE_8K
 *                                TW_UNIT_STRIPE_SIZE_32K
 *                                TW_UNIT_STRIPE_SIZE_128K
 *                                TW_UNIT_STRIPE_SIZE_512K
 *                                TW_UNIT_STRIPE_SIZE_1024K
 *
 *                              For all other unit configurations use
 *                                TW_UNIT_STRIPE_SIZE_UNUSED
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of the new unit.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      layout              - Layout is supported for a unit configuration of
 *                              TW_UNIT_CONFIG_RAID_50.  The number of disks
 *                              per disk-group in a RAID 50 configuration. 
 *                              Must be one of:
 *                                TW_UNIT_CONFIG_RAID_50_G3
 *                                TW_UNIT_CONFIG_RAID_50_G4
 *                                TW_UNIT_CONFIG_RAID_50_G5
 *                                TW_UNIT_CONFIG_RAID_50_G6
 *                                TW_UNIT_CONFIG_RAID_50_G7
 *                                TW_UNIT_CONFIG_RAID_50_G8
 *                                TW_UNIT_CONFIG_RAID_50_G9
 *                                TW_UNIT_CONFIG_RAID_50_G10
 *                                TW_UNIT_CONFIG_RAID_50_G11
 *                                TW_UNIT_CONFIG_RAID_50_G12
 *                                TW_UNIT_CONFIG_RAID_50_G13
 *                                TW_UNIT_CONFIG_RAID_50_G14
 *                                TW_UNIT_CONFIG_RAID_50_G15
 *                                TW_UNIT_CONFIG_RAID_50_G16
 *                              For all other unit configurations use
 *                                TW_UNIT_CONFIG_RAID_50_G_UNUSED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Migration of the specified unit has started.
 *      TW_RET_FAILED       - Migration of the specified unit has NOT started.
 *
 *    Remarks:
 *      Use this routine to migrate a unit to another RAID level, to expand
 *      capacity or both.  This routine will initialize drive media to prepare
 *      it for use.  Any data present on the additional drives will no longer
 *      be accessible once the unit is migrated.  All data present on the
 *      original unit is available during and after the migration.
 *      I/O operations are supported during the migration.
 *
 *    See also:
 *      tw_create_unit
 *      tw_create_unit_with_layout 
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_migrate_unit (
      tw_unit_id_type            unit_id,
      tw_drive_id_list_type*     drive_id_list,
      tw_configuration_type      unit_config,
      tw_stripe_size_type        stripe_size,
      unsigned char              make_os_aware,
      unsigned int               layout
                    ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Splits a mirrored unit into individual drives.  The API verifies that
 *      the unit ID and configuration are valid, then splits the mirror into
 *      single drive units (TW_UNIT_CONFIG_IBOD).  The unit IDs of new units
 *      are written to the location specified by the unit_id_list parameter.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be split.  Must be have a
 *                              TW_UNIT_CONFIG_TWINSTOR unit configuration.
 *      make_os_aware       - Flag to determine whether the API will make the
 *                              operating system aware of new units.
 *                              Must be one of:
 *                                    TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT
 *                                    TW_NOTIFY_OS_OF_NEW_UNIT
 *      new_unit_id_list    - The address of a list of new unit IDs.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is split.
 *      TW_RET_FAILED       - The specified unit is NOT split.
 *
 *    Remarks:
 *      Use this routine if you want to split a mirrored unit into individual
 *      drive units (TW_UNIT_CONFIG_IBOD).
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_split_mirror (
      tw_unit_id_type         unit_id,
      unsigned char           make_os_aware,
      tw_unit_id_list_type*   new_unit_id_list
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Verifies the specified unit.  The API verifies that the unit ID
 *      is valid, then verifies the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be verified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is verified.
 *      TW_RET_FAILED       - The specified unit is NOT verified.
 *
 *    Remarks:
 *      Use this routine to place a particular unit in a verify state.
 *      A verify operation will be performed according to the verify
 *      schedule state.
 *
 *    See also:
 *      tw_get_background_verify_rate
 *      tw_set_background_verify_rate
 *      tw_get_verify_schedule_state
 *      tw_enable_verify_task_schedule
 *      tw_disable_verify_task_schedule
 *      tw_get_verify_task_item
 *      tw_set_verify_task_item
 *      tw_get_task_schedule
 *      tw_set_task_schedule
 *      tw_get_unit_auto_verify_policy
 *      tw_enable_unit_auto_verify          
 *      tw_disable_unit_auto_verify         
 *      tw_stop_verify_unit                 
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_verify_unit ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Stops verify on the specified unit.  The API verifies that the unit ID
 *      is valid, then stops the verify operation on the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Verify operation stopped on the specified unit.
 *      TW_RET_FAILED       - Verify operation unchanged on the specified unit.
 *
 *    Remarks:
 *      Use this routine to stop a verify operation on a particular unit.
 *
 *    See also:
 *      tw_get_verify_schedule_state
 *      tw_get_unit_percentage_complete
 *      tw_verify_unit                 
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_stop_verify_unit ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Rebuilds the specified unit with the specified drives.
 *      The API verifies that the unit ID and drive list are
 *      valid, then rebuilds the unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit to be rebuilt.
 *      drive_id_list       - The address of the list of drives tw_rebuild_unit
 *                              will use to rebuild the unit.
 *                              The drive status must be one of:
 *                                TW_DRIVE_STATUS_OK
 *                                TW_DRIVE_STATUS_DCB_ORPHAN
 *                                TW_DRIVE_STATUS_DCB_DATA_CHECK
 *                                TW_DRIVE_STATUS_UNSUPPORTED_DCB
 *                                TW_DRIVE_STATUS_UNCONVERTED_DCB
 *      ignore_ecc_errors   - The policy of the rebuild operation if ECC errors
 *                              are found.  Must be of:
 *                                TW_HALT_ON_ECC_ERRORS
 *                                TW_CONTINUE_ON_ECC_ERRORS
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit is rebuilt.
 *      TW_RET_FAILED       - The specified unit is NOT rebuilt.
 *
 *    Remarks:
 *      Use this routine to rebuild an existing unit, using available drives,
 *      on a particular controller.  This routine will overwrite drive data
 *      to prepare it for use.  Any data present on the new drives will not
 *      be accessible once the unit is rebuilt.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_add_drive
 *      tw_remove_drive
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_rebuild_unit ( 
      tw_unit_id_type          unit_id,
      tw_drive_id_list_type*   drive_id_list,
      unsigned char            ignore_ecc_errors
                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *
 *    Description:
 *      Pauses rebuild on the specified unit.  The API verifies that
 *      the unit ID is valid, then pauses the rebuild operation on
 *      the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit of interest.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Rebuild operation paused
 *                              on the specified unit.
 *      TW_RET_FAILED       - Rebuild operation unchanged
 *                              on the specified unit.
 *
 *    Remarks:
 *      Use this routine to pause a rebuild operation on a particular unit.
 *      Use the tw_resume_rebuild_unit routine to continue rebuild operation.
 *
 *    See also:
 *      tw_rebuild_unit
 *      tw_resume_rebuild_unit
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_pause_rebuild_unit ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *
 *    Description:
 *      Resumes a paused rebuild operation on the specified unit.
 *      The API verifies that the unit ID is valid, then resumes
 *      the paused rebuild operation on the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit of interest.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Rebuild operation resumed
 *                              on the specified unit.
 *      TW_RET_FAILED       - Rebuild operation unchanged
 *                              on the specified unit.
 *
 *    Remarks:
 *      Use this routine to resume a rebuild operation on a particular unit, 
 *      Use the tw_pause_rebuild_unit routine to pause a rebuild operation.
 *
 *    See also:
 *      tw_rebuild_unit
 *      tw_pause_rebuild_unit
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_resume_rebuild_unit ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series ( 9650 and higher)
 *    Description:
 *      Sets read cache policy setting for the specified unit.  The API
 *      verifies that the unit ID is valid, then sends requests to the
 *      specified unit's controller.
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      policy            - The unit read cache policy setting value,
 *                           must be one of:
 *                              TW_UNIT_READ_CACHE_DISABLED
 *                                 - Disable unit read cache.
 *                              TW_UNIT_READ_CACHE_BASIC
 *                                 - Set unit read cache to basic (No read-ahead).
 *                              TW_UNIT_READ_CACHE_INTELLIGENT
 *                                 - Set unit read cache to intelligent (read-ahead enabled).
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Successful in setting the read cache policy setting
 *                           for the specified unit.
 *      TW_RET_FAILED     - Failed to set the unit read cache policy.
 *                            See tw_get_error_information() to get error details.
 *
 *    Remarks:
 *      Units have an associated read cache.  Applications can use this
 *      routine to set a read cache policy on a particular unit.
 *      Read cache settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_get_unit_read_cache_policy
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_unit_read_cache_policy (
      tw_unit_id_type             unit_id,
      tw_unit_cache_policy_type   policy
      );



/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Flush the write cache on the specified unit.  The API verifies that the
 *      unit ID is valid, then flushes the write cache on the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which to flush write cache.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit write cache is flushed.
 *      TW_RET_FAILED       - The specified unit write cache is NOT flushed.
 *
 *    Remarks:
 *      Use this routine to ensure all data in a particular unit's write cache
 *      is written to drive media.
 *
 *    See also:
 *      tw_get_unit_write_cache_state
 *      tw_enable_unit_write_cache          
 *      tw_disable_unit_write_cache         
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_flush_unit_write_cache ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Enables the write cache on the specified unit.  The API verifies
 *      that the unit ID is valid, then enables the write cache on the
 *      specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which to enable write cache.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit write cache is enabled.
 *      TW_RET_FAILED       - The specified unit write cache is NOT enabled.
 *
 *    Remarks:
 *      Use this routine to enable a particular unit's write cache.
 *
 *    See also:
 *      tw_get_unit_write_cache_state
 *      tw_flush_unit_write_cache
 *      tw_disable_unit_write_cache         
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_unit_write_cache ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Disables the write cache on the specified unit.  The API verifies
 *      that the unit ID is valid, then disables the write cache on the
 *      specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which to disable write cache.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit write cache is disabled.
 *      TW_RET_FAILED       - The specified unit write cache is NOT disabled.
 *
 *    Remarks:
 *      Use this routine to disable a particular unit's write cache.
 *
 *    See also:
 *      tw_get_unit_write_cache_state
 *      tw_flush_unit_write_cache
 *      tw_enable_unit_write_cache         
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_unit_write_cache ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Enables auto verify on the specified unit.  The API verifies
 *      that the unit ID is valid, then enables auto verify on the
 *      specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which to enable auto verify.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit auto verify is enabled.
 *      TW_RET_FAILED       - The specified unit auto verify is NOT enabled.
 *
 *    Remarks:
 *      Use this routine to enable a particular unit's auto verify policy.
 *      Units with the auto verify policy enabled will have a verify operation
 *      performed according to the verify task schedule.  Auto verify policy
 *      settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_get_unit_auto_verify_policy
 *      tw_disable_unit_auto_verify
 *      tw_get_verify_schedule_state
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_unit_auto_verify ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Disables auto verify on the specified unit.  The API verifies
 *      that the unit ID is valid, then disables auto verify on the
 *      specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which to disable auto verify.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit auto verify is disabled.
 *      TW_RET_FAILED       - The specified unit auto verify is NOT disabled.
 *
 *    Remarks:
 *      Use this routine to disable a particular unit's auto verify policy.
 *      Units with the auto verify policy disabled will have a verify operation
 *      performed only when an tw_verify_unit command is called.
 *      Auto verify policy settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_get_unit_auto_verify_policy
 *      tw_enable_unit_auto_verify
 *      tw_get_verify_schedule_state
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_unit_auto_verify ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Sets ECC error policy to "halt on ECC errors" on the specified unit.
 *      The API verifies that the unit ID is valid, then sets the ECC error
 *      policy on the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which the ECC error policy
 *                              will be set.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit ECC error policy
 *                              is set to "halt on ECC errors".
 *      TW_RET_FAILED       - The specified unit ECC error policy
 *                              is unchanged.
 *
 *    Remarks:
 *      Units have an associated ecc policy.  Applications can use this
 *      routine to set a particular unit's ecc policy to halt on ECC errors.
 *      Units with the ecc policy set to halt on ECC errors will stop when
 *      ECC errors are encountered on the source drive(s) during a rebuild
 *      operation.
 *
 *    See also:
 *      tw_get_unit_ecc_policy
 *      tw_unit_set_ecc_policy_to_continue
 *      tw_get_rebuild_schedule_state
 *      tw_get_task_schedule
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_set_ecc_policy_to_halt ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Sets ECC error policy to "continue on ECC errors" on the specified unit.
 *      The API verifies that the unit ID is valid, then sets the ECC error
 *      policy on the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit on which the ECC error policy
 *                              will be set.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified unit ECC error policy
 *                              is set to "continue on ECC errors".
 *      TW_RET_FAILED       - The specified unit ECC error policy
 *                              is unchanged.
 *
 *    Remarks:
 *      Units have an associated ecc policy.  Applications can use this
 *      routine to set a particular unit's ecc policy to continue on ECC
 *      errors.  Units with the ecc policy set to continue on ECC errors
 *      will override ECC errors encountered on the source drive(s) during
 *      a rebuild operation.  This allows a rebuild to complete by ignoring
 *      ECC errors, at the risk of some data.
 *
 *    See also:
 *      tw_get_unit_ecc_policy
 *      tw_unit_set_ecc_policy_to_halt
 *      tw_get_rebuild_schedule_state
 *      tw_get_task_schedule
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_set_ecc_policy_to_continue ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and above)
 *
 *    Description:
 *      Sets the storsave mode of the specified 3ware unit to protection.
 *      The API verifies that the unit ID is valid, then writes
 *      a protection storsave value to the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The storsave mode for the specified unit
 *                              is set to TW_STORSAVE_PROTECTION.
 *      TW_RET_FAILED       - The storsave mode for the specified unit
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to set a particlar unit's level of
 *      data protection vs performance to maximum data protection.
 *
 *    See also:
 *      tw_get_unit_storsave_mode
 *      tw_set_unit_storsave_mode
 *      tw_unit_storsave_balance
 *      tw_unit_storsave_performance
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_storsave_protection ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and above)
 *
 *    Description:
 *      Sets the storsave mode of the specified 3ware unit to balance.
 *      The API verifies that the unit ID is valid, then writes
 *      a balance storsave value to the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The storsave mode for the specified unit
 *                              is set to TW_STORSAVE_BALANCE.
 *      TW_RET_FAILED       - The storsave mode for the specified unit
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to read a particular unit's level of
 *      data protection vs performance to balance data protection.
 *
 *    See also:
 *      tw_get_unit_storsave_mode
 *      tw_set_unit_storsave_mode
 *      tw_unit_storsave_protection
 *      tw_unit_storsave_performance
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_storsave_balance ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and above)
 *
 *    Description:
 *      Sets the storsave mode of the specified 3ware unit to performance.
 *      The API verifies that the unit ID is valid, then writes
 *      a performance storsave value to the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The storsave mode for the specified unit
 *                              is set to TW_STORSAVE_PERFORMANCE.
 *      TW_RET_FAILED       - The storsave mode for the specified unit
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to read a particular unit's level of
 *      data protection vs performance to maximum performance.
 *
 *    See also:
 *      tw_get_unit_storsave_mode
 *      tw_set_unit_storsave_mode
 *      tw_unit_storsave_protection
 *      tw_unit_storsave_balance
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_unit_storsave_performance ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and above)
 *
 *    Description:
 *      Writes the storsave mode of the specified unit.
 *      The API verifies that the unit ID is valid and the storsave mode
 *      is valid, then writes the specified storsave mode of the
 *      specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit in question.
 *      storsave_mode       - The value tw_set_unit_storsave_mode
 *                              will write the storsave mode.
 *                              Value MUST be one of:
 *                                TW_STORSAVE_PROTECTION
 *                                TW_STORSAVE_BALANCE
 *                                TW_STORSAVE_PERFORMANCE
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The storsave mode for the specified unit is
 *                              set to the specified value.
 *      TW_RET_FAILED       - The storsave mode for the specified unit
 *                              is unchanged.
 *
 *    Remarks:
 *      Use this routine to set a particular unit's level of
 *      data protection vs performance:
 *
 *        TW_STORSAVE_PROTECTION  - unit provides maximum data protection.
 *          "Write Cache" will be disabled when the unit becomes "DEGRADED". 
 *          Data in unit cache will be flushed to media.
 *          Host commands with an FUA (Force Unit Access) flag will be ignored
 *            if a BBU is installed and enabled;  Honored otherwise.
 *
 *        TW_STORSAVE_BALANCE     - 
 *          "Write Cache" will not be disabled when the unit becomes "DEGRADED".
 *          Data in unit cache will be flushed to media if a BBU is
 *            installed and enabled; Otherwise, data will be flushed to drive.
 *            Data flushed to the drive may, possibly, be in the drive cache
 *            and not yet on the drive media.
 *          Host commands with an FUA (Force Unit Access) flag will be ignored
 *            if a BBU is installed and enabled;  Honored otherwise.
 *
 *        TW_STORSAVE_PERFORMANCE - unit provides maximum performance.
 *          "Write Cache" will not be disabled when the unit becomes "DEGRADED".
 *          Data in unit cache will be flushed to drive.  Data flushed to
 *            the drive may, possibly, be in the drive cache and not yet on the
 *            drive media.
 *          Host commands with an FUA (Force Unit Access) flag will be honored.
 *
 *    See also:
 *      tw_get_unit_storsave_mode
 *      tw_set_unit_storsave_mode
 *      tw_unit_storsave_protection
 *      tw_unit_storsave_balance
 *      tw_unit_storsave_performance
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_unit_storsave_mode (
      tw_unit_id_type     unit_id,
      tw_storsave_type    storsave_mode
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the queue mode of the specified unit.  The API
 *      verifies that the unit ID is valid, then enables the queue mode
 *      of the specified unit.
 * 
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The queue mode for the specified unit
 *                            is enabled.
 *      TW_RET_FAILED     - The queue mode for the specified unit
 *                            is unchanged.
 *    Remarks:
 *      Units have an associated queue mode.  Applications can use
 *      this routine to enable a particular unit's queue mode state.
 *      Units with the queue mode enabled will have I/O commands use
 *      disk DMA queuing.  Units with the queue mode disabled will
 *      NOT have I/O commands use disk DMA queuing.
 *      Queue mode settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_get_unit_queue_mode
 *      tw_disable_unit_queuing
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *    Serial ATA specifications can be found at http://www.serialata.org
 * </pre>
 *
 */
int tw_enable_unit_queuing ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the queue mode of the specified unit.  The API
 *      verifies that the unit ID is valid, then disables the queue mode
 *      of the specified unit.
 * 
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The queue mode for the specified unit
 *                            is disabled.
 *      TW_RET_FAILED     - The queue mode for the specified unit
 *                            is unchanged.
 *    Remarks:
 *      Units have an associated queue mode.  Applications can use
 *      this routine to disable a particular unit's queue mode state.
 *      Units with the queue mode enabled will have I/O commands use
 *      disk DMA queuing.  Units with the queue mode disabled will
 *      NOT have I/O commands use disk DMA queuing.
 *      Queue mode settings are not persistent for JBOD units.
 *
 *    See also:
 *      tw_get_unit_queue_mode
 *      tw_disable_unit_queuing
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 *      Serial ATA specifications can be found at http://www.serialata.org
 * </pre>
 *
 */
int tw_disable_unit_queuing ( tw_unit_id_type   unit_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *      Description:
 *        Sets the specified name of the specified unit.  The API verifies
 *        that the unit ID is valid, then sets the specified name of the
 *        specified unit.
 *
 *      Parameters:
 *        unit_id           - ID of the unit in question.
 *        name              - The address tw_set_unit_name will
 *                              use to set the unit name.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The name for the specified unit
 *                              is set with the specified value.
 *        TW_RET_FAILED     - The name for the specified unit
 *                              is unchanged.
 *
 *      Remarks:
 *        Each unit has an associated name.  Applications can use
 *        this routine to write a particular unit's name.  Unit names
 *        are operator specified, null terminated, ascii strings.
 *
 *      See also:
 *        tw_get_unit_name
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_set_unit_name (
      tw_unit_id_type      unit_id,
      tw_unit_name_type*   name
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified unit.
 *      The API verifies that the unit ID is valid, then identifies
 *      the ports of the specified unit.
 *
 *    Parameters:
 *      unit_id             - ID of the unit of interest.
 *      on_off              - State the unit's ports should assume.
 *                              Must be one of:
 *                                TW_ON  - unit is identified.
 *                                TW_OFF - unit is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The unit's ports are identified.
 *      TW_RET_FAILED       - The unit's ports are NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify ports associated with
 *      a particular unit.
 *
 *    See also:
 *      tw_get_port_identify_status
 *      tw_identify_port
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_unit (
      tw_unit_id_type           unit_id,
      tw_identify_status_type   on_off
                     );


/*******************************************************************************
 * Drive Configuration and Management Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Adds a drive with the specified port ID.  The API verifies that the
 *      port ID is valid, then adds the drive with the specified port ID to
 *      the system.  The drive ID of the newly added drive is written to the
 *      location specified by the drive_id parameter.
 *
 *    Parameters:
 *      port_id             - The port ID of the drive to be added.
 *      drive_id            - address of the newly added drive ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The drive with the specified port ID
 *                              is available with the returned drive ID.
 *      TW_RET_FAILED       - The drive with the specified port ID
 *                              is NOT available.
 *
 *    Remarks:
 *      Use this routine to add a new drive, to a particular port.
 *      This routine will cause the specified physical port to be
 *      probed by it's controller to detect a newly inserted drive.
 *
 *    See also:
 *      tw_remove_drive
 *      tw_create_unit
 *      tw_import_unit
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_add_drive (
      tw_port_id_type     port_id,
      tw_drive_id_type*   drive_id
                 ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Removes a drive with the specified drive ID.  The API verifies that
 *      the drive ID is valid, then removes the drive with the specified
 *      drive ID from the system.
 *
 *    Parameters:
 *      drive_id            - ID of the drive to be removed.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The drive with the specified drive ID
 *                              is removed from the system.
 *      TW_RET_FAILED       - The drive with the specified drive ID
 *                              is NOT removed from the system.
 *
 *    Remarks:
 *      Use this routine to remove an existing drive, from a particular port.
 *      This routine will cause the drive associated with the specified
 *      physical port to be removed from it's controller's configuration.
 *
 *    See also:
 *      tw_add_drive
 *      tw_destroy_unit
 *      tw_export_unit
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_remove_drive ( tw_drive_id_type   drive_id ); 


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Identify the specified port.
 *      The API verifies that the port ID is valid, then identifies
 *      the port.
 *
 *    Parameters:
 *      port_id             - ID of the port of interest.
 *      on_off              - State the port should assume.
 *                              Must be one of:
 *                                TW_ON  - port is identified.
 *                                TW_OFF - port is NOT identified.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The corresponding port is identified.
 *      TW_RET_FAILED       - The corresponding port is NOT identified.
 *
 *    Remarks:
 *      Use this routine to identify a particular port.
 *
 *    See also:
 *      tw_get_port_identify_status
 *      tw_get_unit_identify_status
 *      tw_identify_unit
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_identify_port (
      tw_port_id_type           port_id,
      tw_identify_status_type   on_off
                     );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      This routine examines a firmware image, a given drive and then provides a
 *      recommendation for Firmware Update. The caller is free to follow the
 *      recommendation or ignore it and proceed with drive firmware download
 * 
 *    Parameters:
 *      drive_id             - ID of the drive in question.
 *      prom_name            - Firmware image file name.
 *      compatibility        - Address of compatibility.
 *                               Defined as:
 *                                 typedef  struct _tw_drive_compatibility_tag
 *                                 {
 *                                   unsigned char           comp;
 *                                   char                    curr_ver[TW_MAX_FIRMWARE_VERSION_LEN];
 *                                   char                    new_ver[TW_MAX_FIRMWARE_VERSION_LEN];
 *                                 } tw_drive_compatibility_type;
 *                            
 *                                "comp" field will be set to one of these values,
 *                                  - TW_DOWNLOAD_MICROCODE_NOT_COMPATIBLE
 *                                      Firmware image is NOT compatible with the drive.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_NEW
 *                                      Firmware image is newer than the current drive firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_OLD
 *                                      Firmware image is older than the current drive firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_SAME
 *                                      Firmware image is of the same version as the current drive firmware.
 *                                  - TW_DOWNLOAD_MICROCODE_VERSION_NOT_KNOWN
 *                                      Firmware image version or the current drive firmware version not known.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified compatiblity contains a recommendation
 *                              for the download of specified firmware image.
 *      TW_RET_FAILED       - Failed to check compatiblity, see tw_get_error_information() for more details.
 *
 *
 *    See also:
 *      tw_drive_firmware_download()
 *      tw_get_drive_firmware_version()
 *      tw_get_drive_id_list()
 * </pre>
 *
 */

int tw_check_drive_image_compatibility (
      tw_drive_id_type                 drive_id,
      char*                            prom_name,
      tw_drive_compatibility_type     *compatibility
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      This routine downloads image file to drive. After the successful download
 *      may require to Reset/Reboot the drive for the downloaded firmware to be active.
 * 
 *    Parameters:
 *      drive_id             - ID of the drive in question.
 *      prom_name            - Firmware image file name.
 *      force                - Skip any validation if is a non-zero value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified firmware image was successfully downloaded.
 *      TW_RET_FAILED       - The specified firmware image was not successfully downloaded.
 *                              see tw_get_error_information() for more details.
 *
 *
 *    See also:
 *      tw_check_drive_image_compatibility()
 *      tw_get_drive_firmware_version()
 *      tw_get_drive_id_list()
 * </pre>
 *
 */

int tw_drive_firmware_download (
      tw_drive_id_type      drive_id,
      char*                 prom_name,
      int                   force
      );


/*******************************************************************************
 * Other Utilities
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *                                          Nvram controller
 *
 *      Description:
 *        Creates a controller ID with the specified controller number.
 *        The API verifies that the controller number is valid, then
 *        creates a controller ID with the specified controller number.
 *        The controller ID is written to the location specified by the
 *        controller_id parameter.
 *
 *        NOTE: It is possible to create an ID for a controller that
 *              does not exist.
 *
 *      Parameters:
 *        controller_number - Number of the controller for the ID.
 *        controller_id     - The address where tw_create_controller_id 
 *                              will store the controller ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The controller ID for the specified controller
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The controller ID for the specified controller
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode a controller ID with a particular
 *        controller number.  tw_get_controller_number is the inverse
 *        function which decodes a controller ID.
 *
 *      See also:
 *        tw_get_controller_number
 *        tw_get_controller_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_controller_id (
      tw_id_base_type          controller_number,
      tw_controller_id_type*   controller_id
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Creates a unit ID with the specified controller number,
 *        unit number, subunit and subsubunit number.
 *        The API verifies that the controller, unit, subunit, and
 *        subsubunit numbers are valid, then creates a unit ID.
 *        The unit ID is written to the location specified by the
 *        unit_id parameter.
 *
 *        NOTE: It is possible to create an ID for a unit that
 *              does not exist.
 *
 *
 *      Parameters:
 *        controller_number     - Number of the controller for the ID.
 *        subsubsubunit_number  - Number of the subsubsubunit for the ID or
 *                                  TW_NULL_UNIT if not used.
 *        unit_number           - Number of the unit for the ID.
 *        subunit_number        - Number of the subunit for the ID or
 *                                  TW_NULL_UNIT if not used.
 *        subsubunit_number     - Number of the subsubunit for the ID or
 *                                  TW_NULL_UNIT if not used.
 *        unit_id               - The address where tw_create_unit_id 
 *                                  will store the unit ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The unit ID for the specified unit
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The unit ID for the specified unit
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode a unit ID with a particular unit
 *        subunit and subsubunit numbers.  tw_get_unit_number is the
 *        inverse function which decodes a unit ID.
 *
 *      See also:
 *        tw_get_unit_number
 *        tw_get_unit_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_unit_id (
      tw_id_base_type    controller_number,
      tw_id_base_type    subsubsubunit_number,
      tw_id_base_type    unit_number,
      tw_id_base_type    subunit_number,
      tw_id_base_type    subsubunit_number,
      tw_unit_id_type*   unit_id
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Creates a drive ID with the specified controller number,
 *        and port number.  The API verifies that the controller
 *        and port numbers are valid, then creates a drive ID.
 *        The drive ID is written to the location specified by the
 *        drive_id parameter.
 *
 *        NOTE: It is possible to create an ID for a drive that
 *              does not exist.
 *
 *
 *      Parameters:
 *        controller_number - Number of the controller for the ID.
 *        port_number       - Number of the port for the ID.
 *        drive_id          - The address where tw_create_drive_id 
 *                              will store the drive ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The drive ID for the specified drive
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The drive ID for the specified drive
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode a drive ID with particular controller
 *        and port number.  tw_get_drive_number is the inverse function
 *        which decodes a drive ID.
 *
 *      See also:
 *        tw_get_drive_number
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_drive_id (
      tw_id_base_type     controller_number,
      tw_id_base_type     port_number,
      tw_drive_id_type*   drive_id
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Creates a port ID with the specified controller number,
 *        and port number.  The API verifies that the controller,
 *        and port numbers are valid, then creates a port ID.
 *        The port ID is written to the location specified by the
 *        port_id parameter.
 *
 *        NOTE: It is possible to create an ID for a port that
 *              does not exist.
 *
 *
 *      Parameters:
 *        controller_number - Number of the controller for the ID.
 *        port_number       - Number of the port for the ID.
 *        port_id           - The address where tw_create_port_id 
 *                              will store the port ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The port ID for the specified port
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The port ID for the specified port
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode a port ID with particular port
 *        number.  tw_get_port_number is the inverse function which
 *        decodes a port ID.
 *
 *      See also:
 *        tw_get_port_number
 *        tw_get_port_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_port_id (
      tw_id_base_type    controller_number,
      tw_id_base_type    port_number,
      tw_port_id_type*   port_id
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9690 and higher)
 *
 *    Description:
 *      Creates a phy ID with the specified controller number,
 *      and phy number.  The API verifies that the controller,
 *      and phy numbers are valid, then creates a phy ID.
 *      The phy ID is written to the location specified by the
 *      phy_id parameter.
 *
 *      NOTE: It is possible to create an ID for a phy that
 *            does not exist.
 *
 *    Parameters:
 *      controller_number - Number of the controller for the ID.
 *      phy_number        - Number of the phy for the ID.
 *      phy_id            - The address where tw_create_phy_id 
 *                            will store the phy ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The phy ID for the specified phy
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The phy ID for the specified phy
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode a phy ID with particular phy
 *      number.  tw_get_phy_number is the inverse function which
 *      decodes a phy ID.
 *
 *    See also:
 *      tw_get_phy_number
 *      tw_get_phy_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_phy_id (
      tw_id_base_type    controller_number,
      tw_id_base_type    phy_number,
      tw_phy_id_type*   phy_id
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Creates an enclosure ID with the specified enclosure number.
 *      The API verifies that the controller and enclosure numbers are
 *      valid, then creates a enclosure ID.  The enclosure ID is written
 *      to the location specified by the enclosure_id parameter.
 *
 *      NOTE: It is possible to create an ID for a enclosure that
 *            does not exist.
 *
 *    Parameters:
 *      controller_number - Number of the controller for the ID.
 *      enclosure_number  - Number of the enclosure for the ID.
 *      enclosure_id      - The address where tw_create_enclosure_id 
 *                            will store the enclosure ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The enclosure ID for the specified enclosure
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The enclosure ID for the specified enclosure
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode an enclosure ID with a particular
 *      enclosure number.  tw_get_enclosure_number is the inverse function
 *      which decodes an enclosure ID.
 *
 *    See also:
 *      tw_get_enclosure_number
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_enclosure_id (
      tw_id_base_type         controller_number,
      tw_id_base_type         enclosure_number,
      tw_enclosure_id_type*   enclosure_id
                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                         9000 series (9650 and higher)
 *
 *    Description:
 *      Creates a fan ID with the specified fan number.
 *      The API verifies that the enclosure ID and fan numbers
 *      are valid, then creates a fan ID.  The fan ID is written to the
 *      location specified by the fan_id parameter.
 *      NOTE: It is possible to create an ID for a fan that
 *            does not exist.
 *
 *
 *    Parameters:
 *      enclosure_id      - The enclosure ID.
 *      fan_number        - Number of the fan for the ID.
 *      fan_id            - The address where tw_create_fan_id 
 *                            will store the fan ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The fan ID for the specified fan
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The fan ID for the specified fan
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode a fan ID with a particular fan number.
 *      tw_get_fan_number is the inverse function which decodes a fan ID.
 *
 *    See also:
 *      tw_get_fan_number
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_fan_id (
      tw_enclosure_id_type  enclosure_id,
      tw_id_base_type       fan_number,
      tw_fan_id_type*       fan_id
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Creates a drive slot ID with the specified drive slot number.
 *      The API verifies the enclosure ID and
 *      drive slot numbers are valid, then creates a drive slot ID.
 *      The drive slot ID is written to the location specified by the
 *      drive_slot_id parameter.
 *
 *      NOTE: It is possible to create an ID for a drive slot that
 *            does not exist.
 *
 *
 *    Parameters:
 *      enclosure_id         - The enclosure ID.
 *      drive_slot_number    - Number of the drive slot for the ID.
 *      drive_slot_id        - The address where tw_create_drive_slot_id 
 *                               will store the drive slot ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The drive slot ID for the specified drive slot
 *                          is available in the specified location.
 *      TW_RET_FAILED   - The drive slot ID for the specified drive slot
 *                          is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode a drive slot ID with a particular
 *      drive slot number.  tw_get_drive_slot_number is the inverse function
 *      which decodes a drive slot ID.
 *
 *    See also:
 *      tw_get_drive_slot_number
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_drive_slot_id (
      tw_enclosure_id_type     enclosure_id,
      tw_id_base_type          drive_slot_number,
      tw_drive_slot_id_type*   drive_slot_id
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Creates a temperature sensor ID with the specified temperature sensor number.
 *      The API verifies the enclosure ID and temperature sensor numbers
 *      are valid, then creates a temperature sensor ID.  The temperature sensor ID
 *      is written to the location specified by the temp_sensor_id parameter.
 *      NOTE: It is possible to create an ID for a temperature sensor that
 *            does not exist.
 *
 *    Parameters:
 *      enclosure_id       - The enclosure ID.
 *      temp_sensor_number - Number of the temperature sensor for the ID.
 *      temp_sensor_id     - The address where tw_create_temp_sensor_id 
 *                             will store the temperature sensor ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The temperature sensor ID for the specified temperature sensor
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The temperature sensor ID for the specified temperature sensor
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode a temperature sensor ID with a particular
 *      temperature sensor number. tw_get_temp_sensor_number is the inverse function
 *      which decodes a temperature sensor ID.
 *
 *    See also:
 *      tw_get_temp_sensor_number
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_temp_sensor_id (
      tw_enclosure_id_type      enclosure_id,
      tw_id_base_type           temp_sensor_number,
      tw_temp_sensor_id_type*   temp_sensor_id
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Creates a power supply ID with the specified power supply number.
 *      The API verifies the enclosure ID and power supply numbers
 *      are valid, then creates a power supply ID.  The power supply ID is written to the
 *      location specified by the power_supply_id parameter.
 *      NOTE: It is possible to create an ID for a power supply that
 *            does not exist.
 *
 *    Parameters:
 *      enclosure_id        - The enclosure ID.
 *      power_supply_number - Number of the power supply for the ID.
 *      power_supply_id     - The address where tw_create_power_supply_id 
 *                              will store the power supply ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The power supply ID for the specified power supply
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The power supply ID for the specified power supply
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to encode a power supply ID with a particular power supply number.
 *      tw_get_power_supply_number is the inverse function which decodes a power supply ID.
 *
 *    See also:
 *      tw_get_power_supply_number
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_create_power_supply_id (
      tw_enclosure_id_type      enclosure_id,
      tw_id_base_type           power_supply_number,
      tw_power_supply_id_type*  power_supply_id
                     );

#ifndef   Example
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Creates a door lock ID with the specified door lock number.
 *        The API verifies the enclosure ID and
 *        door lock numbers are valid, then creates a door lock ID.
 *        The door lock ID is written to the location specified by the
 *        door_lock_id parameter.
 *
 *        NOTE: It is possible to create an ID for a door lock that
 *              does not exist.
 *
 *
 *      Parameters:
 *        enclosure_id      - The enclosure ID.
 *        door_lock_number  - Number of the door lock for the ID.
 *        door_lock_id      - The address where tw_create_door_lock_id 
 *                              will store the door lock ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The door lock ID for the specified door lock
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The door lock ID for the specified door lock
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode a door lock ID with a particular
 *        door lock number.  tw_get_door_lock_number is the inverse function
 *        which decodes a door lock ID.
 *
 *      See also:
 *        tw_get_door_lock_number
 *        tw_get_door_lock_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_door_lock_id (
      tw_enclosure_id_type    enclosure_id,
      tw_id_base_type         door_lock_number,
      tw_door_lock_id_type*   door_lock_id
                           );

#endif /* Example */

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Creates an alarm ID with the specified alarm number.
 *        The API verifies the enclosure ID and
 *        alarm numbers are valid, then creates an alarm ID.
 *        The alarm ID is written to the location specified by the
 *        alarm_id parameter.
 *
 *        NOTE: It is possible to create an ID for an alarm that
 *              does not exist.
 *
 *
 *      Parameters:
 *        enclosure_id      - The enclosure ID.
 *        alarm_number      - Number of the alarm for the ID.
 *        alarm_id          - The address where tw_create_alarm_id 
 *                              will store the alarm ID.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The alarm ID for the specified alarm
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The alarm ID for the specified alarm
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to encode an alarm ID with a particular
 *        alarm number.  tw_get_alarm_number is the inverse function
 *        which decodes an alarm ID.
 *
 *      See also:
 *        tw_get_alarm_number
 *        tw_get_alarm_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_create_alarm_id (
      tw_enclosure_id_type  enclosure_id,
      tw_id_base_type       alarm_number,
      tw_alarm_id_type*     alarm_id
                       );



/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the controller number of the specified controller.
 *      The API verifies that the controller ID is valid, then requests the
 *      controller number of the specified controller from the API.
 *      The controller number is written to the location specified by the
 *      controller_number parameter.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      controller_number - The address where tw_get_controller_number
 *                            will store the controller number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The controller number for the specified controller
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The controller number for the specified controller
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a controller ID with a particular
 *      controller number.  tw_create_controller_id is the inverse
 *      function which encodes a controller ID.
 *      A value of TW_NULL_CONTROLLER indicates that controller does not exist.
 *
 *    See also:
 *      tw_create_controller_id
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_controller_number (
      tw_controller_id_type   controller_id,
      tw_id_base_type*        controller_number
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the unit number of the specified unit.
 *      The API verifies that the unit ID is valid, then requests the
 *      unit number of the specified unit from the API.
 *      The unit number is written to the location specified by the
 *      unit_number parameter.
 *
 *    Parameters:
 *      unit_id              - ID of the unit in question.
 *      controller_number    - The address where tw_get_unit_number
 *                               will store the controller number.
 *      subsubsubunit_number - The address where tw_get_unit_number
 *                               will store the subsubsubunit number.
 *      unit_number          - The address where tw_get_unit_number
 *                               will store the unit number.
 *      subunit_number       - The address where tw_get_unit_number
 *                               will store the subunit number.
 *      subsubunit_number    - The address where tw_get_unit_number
 *                               will store the subsubunit number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The unit number for the specified unit
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The unit number for the specified unit
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a unit ID with a particular unit
 *      subunit and subsubunit numbers.  tw_create_unit_id is the
 *      inverse function which encodes a unit ID.
 *      A unit_number value of TW_NULL_UNIT indicates that unit does not exist.
 *
 *    See also:
 *      tw_create_unit_id
 *      tw_get_unit_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_number (
      tw_unit_id_type    unit_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   subsubsubunit_number,
      tw_id_base_type*   unit_number,
      tw_id_base_type*   subunit_number,
      tw_id_base_type*   subsubunit_number
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the port number of the specified drive ID.
 *        The API verifies that the drive ID is valid, then requests the
 *        controller and port numbers of the specified drive ID from the API.
 *        The controller number is written to the location specified by the
 *        controller_number parameter.  The port number is written to the
 *        location specified by the port_number parameter.
 *
 *      Parameters:
 *        drive_id          - ID of the drive in question.
 *        controller_number - The address where tw_get_drive_number
 *                              will store the controller number.
 *        port_number       - The address where tw_get_drive_number
 *                              will store the port number.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The controller and port numbers for the specified
 *                              drive ID are available in the
 *                              specified location.
 *        TW_RET_FAILED     - The controller and port numbers for the specified
 *                              drive ID are NOT available in the
 *                              specified location.
 *
 *      Remarks:
 *        Use this routine to decode a drive ID.  tw_create_drive_id is the
 *        inverse function which encodes a drive ID.
 *        A value of TW_NULL_DRIVE indicates that drive does not exist.
 *
 *      See also:
 *        tw_create_drive_id
 *        tw_get_drive_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_number (
      tw_drive_id_type   drive_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   port_number
                        );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *      Description:
 *        Provides the port number of the specified port ID.
 *        The API verifies that the port ID is valid, then requests the
 *        port number of the specified port from the API.
 *        The port number is written to the location specified by the
 *        port_number parameter.
 *
 *      Parameters:
 *        port_id             - ID of the port in question.
 *        controller_number   - The address where tw_get_port_number
 *                                will store the controller number.
 *        port_number         - The address where tw_get_port_number
 *                                will store the port number.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The port number for the specified port
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The port number for the specified port
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to decode a port ID with particular port
 *        number.  tw_create_port_id is the inverse function which
 *        encodes a port ID.
 *        A value of TW_NULL_PORT indicates that port does not exist.
 *
 *      See also:
 *        tw_create_port_id
 *        tw_get_port_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_number (
      tw_port_id_type    port_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   port_number
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *      Description:
 *        Provides the phy number of the specified phy ID.
 *        The API verifies that the phy ID is valid, then requests the
 *        phy number of the specified phy from the API.
 *        The phy number is written to the location specified by the
 *        phy_number parameter.
 *
 *      Parameters:
 *        phy_id             - ID of the phy in question.
 *        controller_number   - The address where tw_get_phy_number
 *                                will store the controller number.
 *        phy_number         - The address where tw_get_phy_number
 *                                will store the phy number.
 *
 *      Return Values:
 *        TW_RET_SUCCESS    - The phy number for the specified phy
 *                              is available in the specified location.
 *        TW_RET_FAILED     - The phy number for the specified phy
 *                              is NOT available in the specified location.
 *
 *      Remarks:
 *        Use this routine to decode a phy ID with particular phy
 *        number.  tw_create_phy_id is the inverse function which
 *        encodes a phy ID.
 *        A value of TW_NULL_PHY indicates that phy does not exist.
 *
 *      See also:
 *        tw_create_phy_id
 *        tw_get_phy_id_list
 *        tw_api_defines.h
 *        tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_number (
      tw_phy_id_type    phy_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   phy_number
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a phy ID for the specified port.  The API verifies
 *      that the port ID is valid, then obtains the ID of the phy
 *      associated with the specified port.  The phy ID is written
 *      to the location specified by phy_id.
 *
 *    Parameters:
 *      port_id       - ID of the port in question.
 *      phy_id        - The address where tw_get_phy_id_from_port_id
 *                        will store the phy ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The phy id is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The phy id is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine which phy is connected to
 *      a particular port.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_phy_id_from_port_id (
      tw_port_id_type      port_id,
      tw_phy_id_type*      phy_id
                               );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a port ID for the specified phy.  The API verifies
 *      that the phy ID is valid, then obtains the ID of the port
 *      associated with the specified phy.  The port ID is written
 *      to the location specified by port_id.
 *
 *    Parameters:
 *      phy_id       - ID of the phy in question.
 *      port_id      - The address where tw_get_port_id_from_phy_id
 *                       will store the port ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The port id is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The port id is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine which phy is connected to
 *      a particular port.
 *
 *    See also:
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_id_from_phy_id (
      tw_phy_id_type      phy_id,
      tw_port_id_type*    port_id
                               );



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the enclosure number of the specified enclosure.
 *      The API verifies that the enclosure ID is valid, then requests the
 *      enclosure number of the specified enclosure from the API.
 *      The enclosure number is written to the location specified by the
 *      enclosure_number parameter.
 *
 *    Parameters:
 *      enclosure_id        - ID of the enclosure in question.
 *      controller_number   - The address where tw_get_enclosure_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_enclosure_number
 *                              will store the enclosure number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure number for the specified enclosure
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The enclosure number for the specified enclosure
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode an enclosure ID with particular enclosure
 *      number.  tw_create_enclosure_id is the inverse function which
 *      encodes an enclosure ID.
 *      A enclosure number value of TW_NULL_ENCLOSURE indicates that enclosure does not exist.
 *
 *    See also:
 *      tw_create_enclosure_id
 *      tw_get_enclosure_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_enclosure_number (
      tw_enclosure_id_type   enclosure_id,
      tw_id_base_type*       controller_number,
      tw_id_base_type*       enclosure_number
                            );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the fan number of the specified fan.
 *      The API verifies that the fan ID is valid, then requests the
 *      fan number of the specified fan from the API.
 *      The fan number is written to the location specified by the
 *      fan_number parameter.
 *
 *    Parameters:
 *      fan_id              - ID of the fan in question.
 *      controller_number   - The address where tw_get_fan_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_fan_number
 *                              will store the enclosure number.
 *      fan_number          - The address where tw_get_fan_number
 *                              will store the fan number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The fan number for the specified fan
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The fan number for the specified fan
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a fan ID with particular fan
 *      number.  tw_create_fan_id is the inverse function which
 *      encodes a fan ID.
 *      A fan number value of TW_NULL_FAN indicates that fan does not exist.
 *
 *    See also:
 *      tw_create_fan_id
 *      tw_get_fan_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fan_number (
      tw_fan_id_type     fan_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   enclosure_number,
      tw_id_base_type*   fan_number
                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the drive slot number of the specified drive slot.
 *      The API verifies that the drive slot ID is valid, then requests the
 *      drive slot number of the specified drive slot from the API.
 *      The drive slot number is written to the location specified by the
 *      drive_slot_number parameter.
 *
 *    Parameters:
 *      drive_slot_id       - ID of the drive slot in question.
 *      controller_number   - The address where tw_get_drive_slot_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_drive_slot_number
 *                              will store the enclosure number.
 *      drive_slot_number   - The address where tw_get_drive_slot_number
 *                              will store the drive_slot number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive slot number for the specified drive slot
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The drive slot number for the specified drive slot
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a drive slot ID with particular drive slot
 *      number.  tw_create_drive_slot_id is the inverse function which
 *      encodes a drive slot ID.
 *      A drive slot value of TW_NULL_DRIVE_SLOT indicates that drive slot does not exist.
 *
 *    See also:
 *      tw_create_drive_slot_id
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_slot_number (
      tw_drive_slot_id_type    drive_slot_id,
      tw_id_base_type*         controller_number,
      tw_id_base_type*         enclosure_number,
      tw_id_base_type*         drive_slot_number
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the temperature sensor number of the specified temperature sensor.
 *      The API verifies that the temperature sensor ID is valid, then requests the
 *      temperature sensor number of the specified temperature sensor from the API.
 *      The temperature sensor number is written to the location specified by the
 *      temp_sensor_number parameter.
 *
 *    Parameters:
 *      temp_sensor_id      - ID of the temperature sensor in question.
 *      controller_number   - The address where tw_get_temp_sensor_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_temp_sensor_number
 *                              will store the enclosure number.
 *      temp_sensor_number  - The address where tw_get_temp_sensor_number
 *                              will store the temperature sensor number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The temperature sensor number for the specified temperature
 *                            sensor is available in the specified location.
 *      TW_RET_FAILED     - The temperature sensor number for the specified temperature
 *                            sensor is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a temperature sensor ID with particular temperature sensor
 *      number.  tw_create_temp_sensor_id is the inverse function which
 *      encodes a temperature sensor ID.
 *      A temperature sensor number value of TW_NULL_TEMP_SENSOR indicates that
 *      sensor does not exist.
 *
 *    See also:
 *      tw_create_temp_sensor_id
 *      tw_get_temp_sensor_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_temp_sensor_number (
      tw_temp_sensor_id_type   temp_sensor_id,
      tw_id_base_type*         controller_number,
      tw_id_base_type*         enclosure_number,
      tw_id_base_type*         temp_sensor_number
                              );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the power supply number of the specified power supply.
 *      The API verifies that the power supply ID is valid, then requests the
 *      power supply number of the specified power supply from the API.
 *      The power supply number is written to the location specified by the
 *      power_supply_number parameter.
 *
 *    Parameters:
 *      power_supply_id     - ID of the power supply in question.
 *      controller_number   - The address where tw_get_power_supply_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_power_supply_number
 *                              will store the enclosure number.
 *      power_supply_number - The address where tw_get_power_supply_number
 *                              will store the power supply number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The power supply number for the specified power supply
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The power supply number for the specified power supply
 *                              is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a power supply ID with particular power supply
 *      number.  tw_create_power_supply_id is the inverse function which
 *      encodes a power supply ID.
 *      A power supply value of TW_NULL_POWER_SUPPLY indicates that power supply does not exist.
 *
 *    See also:
 *      tw_create_power_supply_id
 *      tw_get_power_supply_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_power_supply_number (
      tw_power_supply_id_type   power_supply_id,
      tw_id_base_type*          controller_number,
      tw_id_base_type*          enclosure_number,
      tw_id_base_type*          power_supply_number
                               );


#ifndef   Example
/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the door lock number of the specified door lock.
 *      The API verifies that the door lock ID is valid, then requests the
 *      door lock number of the specified door lock from the API.
 *      The door lock number is written to the location specified by the
 *      door_lock_number parameter.
 *
 *    Parameters:
 *      door_lock_id        - ID of the door lock in question.
 *      controller_number   - The address where tw_get_door_lock_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_door_lock_number
 *                              will store the enclosure number.
 *      door_lock_number    - The address where tw_get_door_lock_number
 *                              will store the door lock number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The door lock number for the specified door lock
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The door lock number for the specified door lock
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode a door lock ID with particular door lock
 *      number.  tw_create_door_lock_id is the inverse function which
 *      encodes a door lock ID.
 *      A door lock number value of TW_NULL_DOOR_LOCK indicates that door lock does not exist.
 *
 *    See also:
 *      tw_create_door_lock_id
 *      tw_get_door_lock_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_door_lock_number (
      tw_door_lock_id_type    door_lock_id,
      tw_id_base_type*        controller_number,
      tw_id_base_type*        enclosure_number,
      tw_id_base_type*        door_lock_number
                            );

#endif /* Example */


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides the alarm number of the specified alarm.
 *      The API verifies that the alarm ID is valid, then requests the
 *      alarm number of the specified alarm from the API.
 *      The alarm number is written to the location specified by the
 *      alarm_number parameter.
 *
 *    Parameters:
 *      alarm_id            - ID of the alarm in question.
 *      controller_number   - The address where tw_get_alarm_number
 *                              will store the controller number.
 *      enclosure_number    - The address where tw_get_alarm_number
 *                              will store the enclosure number.
 *      alarm_number        - The address where tw_get_alarm_number
 *                              will store the alarm number.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The alarm number for the specified alarm
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The alarm number for the specified alarm
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to decode an alarm ID with particular alarm
 *      number.  tw_create_alarm_id is the inverse function which
 *      encodes a alarm ID.
 *      A alarm number value of TW_NULL_ALARM indicates that alarm does not exist.
 *
 *    See also:
 *      tw_create_alarm_id
 *      tw_get_alarm_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_alarm_number (
      tw_alarm_id_type   alarm_id,
      tw_id_base_type*   controller_number,
      tw_id_base_type*   enclosure_number,
      tw_id_base_type*   alarm_number
                        );



/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides the unit ID of the specified drive.  The API verifies that
 *      the drive ID is valid, then requests the unit ID of the specified
 *      drive from the drive's controller.  The unit ID of the drive is
 *      written to the location specified by the unit ID parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      unit_id           - The address where tw_get_unit_id_from_drive_id
 *                            will store the drive's unit ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The unit ID for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The unit ID for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine which, if any, units a particular
 *      drive is a member.  TW_NULL_UNIT is returned when a drive is not
 *      a member of any unit.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_get_drive_id_list_from_unit_id
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_id_from_drive_id (
      tw_drive_id_type   drive_id,
      tw_unit_id_type*   unit_id
                                 );


/**
 * <pre>
 *    Supported Controllers:
 *                                          7000 series
 *                                          8000 series
 *                                          9000 series
 *
 *    Description:
 *      Provides a list of drives from the unit.  The API verifies that the
 *      unit ID is valid, then requests the number and IDs of all drives
 *      from the specified unit.  The drive IDs are written to the location
 *      specified by drive_id_list.  The number of drives on the specified
 *      controller is written to the length element of the drive id list.
 *
 *    Parameters:
 *      unit_id           - ID of the unit in question.
 *      drive_id_list     - The address where tw_get_drive_id_list
 *                             will store the drive id list.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive id list and length are
 *                            available in the specified location.
 *      TW_RET_FAILED     - The drive id list is NOT valid and
 *                            the list length is set to zero.
 *
 *    Remarks:
 *      Use this routine to determine which drives comprise a particular unit.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_get_unit_id_from_drive_id
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_id_list_from_unit_id (
      tw_unit_id_type          unit_id,
      tw_drive_id_list_type*   drive_id_list
                                      );

// This is not supported.
int tw_get_drive_id_from_parity_id (
      tw_parity_id_type      parity_id,
      tw_drive_id_type*      drive_id
                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a drive slot ID for the specified port.  The API verifies
 *      that the port ID is valid, then obtains the ID of the drive slot
 *      associated with the specified port.  The drive slot ID is written
 *      to the location specified by drive_slot_id.
 *
 *    Parameters:
 *      port_id           - ID of the port in question.
 *      drive_slot_id     - The address where
 *                            tw_get_drive_slot_id_from_port_id
 *                            will store the drive slot ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The drive slot id is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The drive slot id is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine which drive slot is connected to
 *      a particular port.
 *
 *    See also:
 *      tw_get_port_id_from_drive_slot_id
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_drive_slot_id_from_port_id (
      tw_port_id_type          port_id,
      tw_drive_slot_id_type*   drive_slot_id
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a port ID for the specified drive slot .  The API verifies
 *      that the drive slot ID is valid, then obtains the ID of the port
 *      associated with the specified drive slot.  The port ID is written
 *      to the location specified by port_id.
 *
 *    Parameters:
 *      drive_slot_id     - ID of the drive slot in question.
 *      port_id           - The address where
 *                             tw_get_port_id_from_drive_slot_id
 *                             will store the port ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The port id is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The port id list is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine which port is connected to
 *      a particular drive slot.
 *
 *    See also:
 *      tw_get_drive_slot_id_from_port_id
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_port_id_from_drive_slot_id (
      tw_drive_slot_id_type   drive_slot_id,
      tw_port_id_type*        port_id
                                      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9650 and higher)
 *
 *    Description:
 *      Provides a enclosure ID for the specified drive slot .  The API verifies
 *      that the drive slot ID is valid, then obtains the ID of the enclosure
 *      associated with the specified drive slot.  The enclosure ID is written
 *      to the location specified by enclosure_id.
 *
 *    Parameters:
 *      drive_slot_id       - ID of the drive slot in question.
 *      enclosure_id        - The address where
 *                             tw_get_enclosure_id_from_drive_slot_id
 *                             will store the enclosure ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The enclosure id is available
 *                            in the specified location.
 *      TW_RET_FAILED     - The enclosure id list is NOT available
 *                            in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine to which enclosure a particular
 *      drive slot belongs.
 *
 *    See also:
 *      tw_get_drive_slot_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */

int tw_get_enclosure_id_from_drive_slot_id (    
      tw_drive_slot_id_type    drive_slot_id,
      tw_enclosure_id_type*    enclosure_id
                                           );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *    Description:
 *      Provides the unit ID given an OS dependent disk identifier as
 *      as an ASCII string. For windows, the disk identifier is an string
 *      containing an integer disk number (this number is also seen
 *      in the Disk Manager). For linux, the disk identifier would be
 *      the disks device entry (such as, "/dev/sdb"). The unit ID of
 *      the given disk number is written to the location specified by
 *      the unit ID parameter.
 *
 *    Parameters:
 *      disk_name         - OS dependent disk identifier or name.
 *      unit_id           - The address where tw_get_unit_id_from_drive_id
 *                            will store the unit ID.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The unit ID of the given disk name
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The unit ID of the given disk name
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      This routine applicable to Windows OS only.
 *
 *    See also:
 *      tw_get_unit_id_list
 *      tw_get_drive_id_list
 *      tw_get_drive_id_list_from_unit_id
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_unit_id_from_disk_name (
      const char         *disk_name,
      tw_unit_id_type*   unit_id
                                 );



/*******************************************************************************
 * Performance Monitoring Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the state of the performance monitoring on the specified controller.
 *   .  The API verifies that the controller ID is valid, then sends the request to the
 *      specified controller. The performance monitoring state is written to the location
 *      specified by the pmstat_state parameter.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      pmstat_state        - The address where tw_get_fw_pmstat_enable_state
 *                              will store the result.  Returns one of:
 *                                TW_PMSTAT_IS_ENABLED
 *                                TW_PMSTAT_IS_DISABLED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully got the performance monitoring state.
 *      TW_RET_FAILED       - Failed to get the performance monitoring state.
 *
 *    Remarks:
 *      The performance monitoring feature of the controller can be enabled or disabled.
 *      Use this routine to read the performance monitoring ON/OFF status of a particular controller
 *
 *    See also:
 *      tw_pmstat_enable
 *      tw_pmstat_disable
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_enable_state (
      tw_controller_id_type   controller_id,
      tw_pmstat_state_type*   pmstat_state
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Enables the performance monitoring on the specified controller.
 *      The API verifies that the controller ID is valid, then enables the
 *      performance monitoring on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The performance monitoring is enabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The performance monitoring state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      The performance monitoring feature of the controller can be enabled or disabled.
 *      Use this routine to enable the performance monitoring feature of a particular controller
 *
 *    See also:
 *      tw_pmstat_disable
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_fw_pmstat_enable ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Disables the performance monitoring on the specified controller.
 *      The API verifies that the controller ID is valid, then disables the
 *      performance monitoring on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The performance monitoring is disabled on
 *                            the specified controller.
 *      TW_RET_FAILED     - The performance monitoring state is unchanged on
 *                            the specified controller.
 *
 *    Remarks:
 *      The performance monitoring feature of the controller can be enabled or disabled.
 *      Use this routine to disable the performance monitoring feature of a particular controller
 *
 *    See also:
 *      tw_pmstat_enable
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_fw_pmstat_disable ( tw_controller_id_type   controller_id );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the performance monitoring capabilities supported by the controller.
 *      The performance can be measured at the unit level or at the individual drives depending upon
 *      how the controller's PM control settings are set. see API tw_set_fw_pmstat_control_setting()
 *      for more details.
 * 
 *    Parameters:
 *      controller_id      - ID of the controller in question.
 *      pmstat_config      - The address where tw_get_fw_pmstat_configuration
 *                              will store the performance monitoring capabilities
 *
 *    Return Values:
 *      TW_RET_SUCCESS   - The performance monitoring capabilities supported by the 
 *                                             specified controller is available in the specified location.
 *      TW_RET_FAILED    - The performance monitoring capabilities supported by the 
 *                                             specified controller is NOT available in the specified location.
 *
 *    Remarks:
 *      Use this routine to determine performance monitoring capabilities supported by the controller.
 *
 *    See also:
 *      tw_get_controller_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_configuration (
      tw_controller_id_type     controller_id,
      tw_pmstat_config_type*    pmstat_config
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the current performance monitoring control setting of the specified drive.
 *      The API verifies that the drive ID is valid, then sends the request
 *      to the specified drive's controller. The control setting is written to the location
 *      specified by the pmstat_control parameter.
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      pmstat_control    - The address where the tw_get_fw_pmstat_control_setting() will
 *                            store the control setting
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The control setting of the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The control setting of the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine get the performance monitoring setting of the controller.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_control_setting (
      tw_controller_id_type      controller_id,
      tw_pmstat_control_type*    pmstat_control
                                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Sets the performance monitoring control setting of the specified drive.
 *      The API verifies that the drive ID is valid, then sends the request
 *      to the specified drive's controller. 
 *
 *    Parameters:
 *      controller_id     - ID of the controller in question.
 *      pmstat_control    - The address of the performance monitoring control setting.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Is successful in setting PM control for the specified drive
 *      TW_RET_FAILED     - Is NOT successful in setting PM control for the specified drive
 *
 *    Remarks:
 *      Applications can use this routine to control performance monitoring and statistics
 *      gathering of the controller.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_fw_pmstat_control_setting (
      tw_controller_id_type     controller_id,
      tw_pmstat_control_type*   pmstat_control
                                     );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the running average of performance statistics of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the performance statistics
 *      of the specified drive from the controller.  The running average
 *      of performance statistics. is written to the location specified by the pmstat_summary parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      pmstat_summary    - The address where tw_fw_get_pmstat_summary will
 *                            store the statistics data.
 *      stor_type         - The requested statistics must be one of:
 *                              TW_PMSTAT_VOLATILE
 *                              TW_PMSTAT_PERSISTENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The running average of performance statistics for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The running average of performance statistics for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the running average of performance statistics.
 *      The volatile data is stored on controller memory that will be lost if rebooted.
 *      The persistent data is stored on the drive that will be preserved during reboot. 
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_summary (
      tw_drive_id_type          drive_id,
      tw_pmstat_summary_type*   pmstat_summary,
      int                       stor_type
                             );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the response histogram data of performance statistics of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the hist data
 *      of the specified drive from the controller and is written to the location
 *      specified by the hist_data parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      hist_data         - The address where tw_get_fw_pmstat_resp_histogram will
 *                            store the hist data.
 *      stor_type         - The requested statistics must be one of:
 *                              TW_PMSTAT_VOLATILE
 *                              TW_PMSTAT_PERSISTENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The hist data for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The hist data for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain response histogram data of performance statistics.
 *      The volatile data is stored on controller memory that will be lost if rebooted.
 *      The persistent data is stored on the drive that will be preserved during reboot. 
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_resp_histogram (
      tw_drive_id_type            drive_id,
      tw_pmstat_resp_histogram*   hist_data,
      int                         stor_type
                                    );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the performance data and command details of IOs that had higher response time
 *      by the specified drive. The API verifies that the drive ID is valid, then sends the request.
 *      to the drive's controller.  The statistics information is written to the location
 *      specified by the pmstat_high_rs parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      pmstat_high_rs    - The address where tw_get_fw_pmstat_high_latency() will
 *                            store the statistics data.
 *      stor_type         - The requested statistics must be one of:
 *                              TW_PMSTAT_VOLATILE
 *                              TW_PMSTAT_PERSISTENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The performance statistics for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The performance statistics for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the performance data of the last N commands
 *      that had higher response time. The volatile data is stored on controller memory which
 *      will be lost if rebooted. The persistent data is stored on the drive and will be preserved if  
 *      rebooted. 
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_high_latency (
      tw_drive_id_type           drive_id,
      tw_pmstat_high_rs_type*    pmstat_high_rs,
      int                        stor_type
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the instantaneous performance statistics of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the performance statistics
 *      of the specified drive from the drive's controller.  The statistics information is
 *      written to the location specified by the pmstat_instant parameter.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      pmstat_instant    - The address where tw_get_fw_pmstat_instantaneous() will
 *                            store the statistics data.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The instantaneous performance statistics for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The instantaneous performance statistics for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the instantaneous performance statistics. 
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_instantaneous (
      tw_drive_id_type          drive_id,
      tw_pmstat_instant_type*   pmstat_instant
                                   );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Clears the performance statistics of the specified drive.
 *      The API verifies that the drive ID is valid, then sends request
 *      to the specified drive's controller to clear the performance statistics data.
 *
 *    Parameters:
 *      drive_id          - ID of the drive in question.
 *      pmstat_type       - The requested statistics can be one of:
 *                              TW_PMSTAT_RUN_AVERAGE_SUMMARY
 *                              TW_PMSTAT_HIGHER_RESPONSE_TIME_CMDS
 *                              TW_PMSTAT_EXTD_DRIVE_STATISTICS (store_type parameter not applicable) 
 *      stor_type         - The requested statistics can be one of:
 *                              TW_PMSTAT_VOLATILE
 *                              TW_PMSTAT_PERSISTENT
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The performance statistics for the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The performance statistics for the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to performance statistics of a particular drive.
 *
 *    See also:
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_fw_pmstat_clear (
     tw_drive_id_type   drive_id,
     int                pmstat_type,
     int                stor_type
                       );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series (9550 and higher)
 *
 *    Description:
 *      Provides the extended drive statistics of the specified drive.
 *      The API verifies that the drive ID is valid, then requests the extended drive statistics
 *      of the specified drive from the drive's controller.  The statistics information is
 *      written to the location specified by the extd_statistics parameter.
 *
 *    Parameters:
 *      drive_id           - ID of the drive in question.
 *      extd_statistics    - The address where tw_get_fw_pmstat_extd_drive_statistics() will
 *                            store the statistics data.
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - The extended drive statistics of the specified drive
 *                            is available in the specified location.
 *      TW_RET_FAILED     - The extended drive statistics of the specified drive
 *                            is NOT available in the specified location.
 *
 *    Remarks:
 *      Applications can use this routine to obtain the extended drive statistics of a particular
 *      drive. The stastics information like the total number of sectors read/written, total
 *      number of read/write/flush commands issued to drive etc.

 *      NOTE: The statistics are updated by the controller only when drive statistics is enabled.
 *             see tw_pmstat_enable() for details on how to enable/disable drive statistics.
 *
 *    See also:
 *      tw_pmstat_enable
 *      tw_pmstat_disable
 *      tw_fw_pmstat_clear
 *      tw_get_drive_id_list
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_fw_pmstat_extd_drive_statistics (
      tw_drive_id_type                        drive_id,
      tw_pmstat_extd_drive_statistics_type*   extd_statistics
                                           );




/*******************************************************************************
 * Advanced Event Capture Functions
 ******************************************************************************/

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Enables the event capture for the specified controller. When a capture is
 *      started, the controller will capture up to number_of_events events.
 *      The event capture can be used with the IO Pattern Analysis tools to
 *      help diagnose performance issues.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      number_of_events    - The maximum number of events that the controller will
 *                              capture. Specify "1" to let firmware choose the default
 *                              number of events. Specifying "0" will return an error
 *                              (see tw_disable_event_capture).
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The event capture for the specified controller
 *                              is now enabled.
 *      TW_RET_FAILED       - The event capture for the specified controller
 *                              could not be enabled. 
 *
 *    Remarks:
 *      Call this routine to allocate and initialize the event capture buffer. This
 *      routine does not start capturing events; it only reserves internal buffer
 *      space for saving the events.
 *
 *    See also:
 *      tw_start_event_capture
 *      tw_set_event_capture_mode
 *      tw_disable_event_capture
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_enable_event_capture (
      tw_controller_id_type     controller_id,
      int                       number_of_events
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Disables the event capture for the specified controller. This will clear
 *      all captured events and free up the controller's internal buffer memory.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The event capture for the specified controller
 *                              is disabled.
 *
 *    Remarks:
 *      Call this routine to disable the event capture and free up the controller's
 *      internal buffer memory. Be sure to retrieve any important event capture
 *      segments BEFORE disabling the event capture.
 *
 *    See also:
 *      tw_stop_event_capture
 *      tw_get_event_capture_length
 *      tw_get_event_capture_segment
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_disable_event_capture (
      tw_controller_id_type     controller_id
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets the capture mode to tell the controller when to stop capturing. Users can
 *      either have the controller automatically stop capturing as soon as the controller
 *      has recorded the maximum number of events (when the buffer is full), or stop
 *      capturing manually by calling tw_stop_event_capture.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      capture_mode        - The value MUST be one of:
 *                                TW_EVENT_CAPTURE_UNTIL_FULL,
 *                                TW_EVENT_CAPTURE_UNTIL_STOPPED
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The capture mode was successfully set.
 *      TW_RET_FAILED       - The capture mode could not be changed.
 *
 *    Remarks:
 *      Make sure capturing is stopped before changing the capture mode.
 *
 *        TW_EVENT_CAPTURE_UNTIL_FULL    - Capture the FIRST number_of_events events.
 *          number_of_events is specified as a parameter to tw_enable_event_capture.
 *          Capturing will stop automatically as soon as the buffer is full.
 *
 *        TW_EVENT_CAPTURE_UNTIL_STOPPED - Capture the LAST number_of_events events.
 *          number_of_events is specified as a parameter to tw_enable_event_capture.
 *          Capturing must be manually stopped by calling tw_stop_event_capture.
 *          If the buffer fills up, older events will be removed so that new events can
 *          always be added.
 *
 *    See also:
 *      tw_enable_event_capture
 *      tw_start_event_capture
 *      tw_stop_event_capture
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_event_capture_mode (
      tw_controller_id_type                 controller_id,
      tw_event_capture_mode_type    capture_mode
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Starts capturing the events on the specified controller. If the capture
 *      mode is TW_EVENT_CAPTURE_UNTIL_FULL, the controller will automatically stop
 *      capturing as soon as the buffer is full. Otherwise the controller will capture
 *      until tw_stop_event_capture is called.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified controller is capturing events.
 *      TW_RET_FAILED       - Capturing events on the specified controller
 *                              could not be started.
 *
 *    Remarks:
 *      Be sure to enable the event capture by calling tw_enable_event_capture before
 *      starting a capture.
 *      Calling this routine causes the controller to overwrite any previous captures.
 *
 *    See also:
 *      tw_enable_event_capture
 *      tw_stop_event_capture
 *      tw_set_event_capture_mode
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_start_event_capture (
      tw_controller_id_type     controller_id
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Stops capturing events on the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The specified controller has stopped capturing events.
 *
 *    Remarks:
 *      If the capture mode is TW_EVENT_CAPTURE_UNTIL_FULL, the controller will
 *      automatically stop capturing as soon as the buffer is full, but you can use
 *      this routine to stop the capture early, before the buffer is full.
 *      After a capture is stopped, it can be retrieved by calling tw_get_event_capture_length
 *      and then tw_get_event_capture_segment.
 *
 *    See also:
 *      tw_enable_event_capture
 *      tw_start_event_capture
 *      tw_set_event_capture_mode
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_stop_event_capture (
      tw_controller_id_type     controller_id
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the number of segments that are available in the event capture
 *      of the specified controller. The event capture can be used with the IO
 *      Pattern Analysis tools to help diagnose performance issues.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      segment_length      - The address where tw_get_event_capture_length will
 *                              store the size of each segment (in bytes).
 *      number_of_segments  - The address where tw_get_event_capture_length will
 *                              store the number of segments available in the
 *                              event capture.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The event capture for the specified controller
 *                              is available.
 *      TW_RET_FAILED       - The event capture for the specified controller
 *                              is NOT available.
 *
 *    Remarks:
 *      Use this routine to determine how many event capture segments can be retrieved.
 *      Then call tw_get_event_capture_segment repeatedly to retrieve each segment.
 *      Capturing must be stopped before retrieving the event capture segments.
 *      Each segment has a header and may contain many events.
 *
 *    See also:
 *      tw_get_event_capture_segment
 *      tw_start_event_capture
 *      tw_set_event_capture_mode
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_length (
      tw_controller_id_type     controller_id,
      int                      *segment_length,
      int                      *number_of_segments
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the event capture of the specified controller. The event
 *      capture can be used with the IO Pattern Analysis tools to help diagnose
 *      performance issues.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      segment_number      - The index of the segment, from 0 to number_of_segments
 *                              returned from tw_get_event_capture_length.
 *      segment             - The address where this routine will store
 *                              the event capture segment. Each segment
 *                              starts with a segment header followed by event data.
 *      max_segment_length  - The size of the segment buffer. This should be the same
 *                              as the segment_length returned from tw_get_event_capture_length.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The event capture for the specified controller
 *                              is available in the specified location.
 *      TW_RET_FAILED       - The event capture segment for the specified controller
 *                              is NOT available or the controller is still capturing.
 *
 *    Remarks:
 *      Use this routine to read a particular controller's event capture.
 *      The event capture must first be enabled and then it must be
 *      started and stopped before retrieving the events.
 *      Call tw_get_event_capture_length to determine how many segments
 *      are available and the segment length. Then call this routine repeatedly
 *      to retrieve each segment. If the capture has not stopped yet, this routine
 *      will return an error; stop the capture before trying to retrieve the events.
 *
 *    See also:
 *      tw_get_event_capture_length
 *      tw_enable_event_capture
 *      tw_start_event_capture
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_segment (
      tw_controller_id_type             controller_id,
      int                               segment_number,
      char                             *segment,
      int                               max_segment_length
                                  );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides the event capture mode setting of the specified 3ware controller.
 *      The API verifes the contrlller ID is valid, then sends request to read
 *      the controller event capture mode setting.
 *
 *    Parameters:
 *      controller_id  - ID of the controller in question.
 *      capture_mode   - The address where the capture mode value is returned
 *                         and will be one of:
 *                           TW_EVENT_CAPTURE_UNDEFINED
 *                           TW_EVENT_CAPTURE_UNTIL_FULL
 *                           TW_EVENT_CAPTURE_UNTIL_STOPPED
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - The capture mode is successfully read.
 *      TW_RET_FAILED   - The capture mode could not be read.
 *
 *    Remarks:
 *        TW_EVENT_CAPTURE_UNTIL_FULL    - Capture the FIRST number_of_events events.
 *          number_of_events is specified as a parameter to tw_enable_event_capture.
 *          Capturing will stop automatically as soon as the buffer is full.
 *
 *        TW_EVENT_CAPTURE_UNTIL_STOPPED - Capture the LAST number_of_events 
 *         events. number_of_events is specified as a parameter to tw_enable_event_capture.
 *          Capturing must be manually stopped by calling tw_stop_event_capture.
 *          If the buffer fills up, older events will be removed so that new events can
 *          always be added.
 *
 *    See also:
 *      tw_set_event_capture_mode
 *      tw_enable_event_capture
 *      tw_start_event_capture
 *      tw_stop_event_capture
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_mode (
      tw_controller_id_type         controller_id,
      tw_event_capture_mode_type   *capture_mode
      );



/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides enable/disalbe sate of the event capture for the specified controller.
 *
 *    Parameters:
 *      controller_id      - ID of the controller in question.
 *      enable_state      -  The address where the value is returned
 *                              and will be one of:
 *                                TW_EVENT_CAPTURE_DISABLED
 *                                TW_EVENT_CAPTURE_ENABLED
 *
 *      number_of_events_set_to_capture    - The maximum number of events that the
 *                              controller will capture. The value of "1"  means, let controller choose
 *                              the default number of events depending upon the availability of controller
 *                              memory buffer.. 
 *
 *    Return Values:
 *      TW_RET_SUCCESS  - Successful in getting the event capture setting of the specified controller
 *      TW_RET_FAILED   - Failed to get the event capture setting for the specified controller.
 *
 *    Remarks:
 *
 *    See also:
 *      tw_enable_event_capture
 *      tw_disable_event_capture
 *      tw_start_event_capture
 *      tw_set_event_capture_mode
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_enable_state(
      tw_controller_id_type          controller_id,
      tw_event_capture_enable_type  *enable_state,
      int                           *number_of_events_to_capture
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides event capture start/stop control state of the specified controller.
 *
 *    Parameters:
 *      controller_id      - ID of the controller in question.
 *      control_state      - The address where the tw_get_event_capture_control_state
 *                             will store the value and is one of:
 *                               TW_EVENT_CAPTURE_STOP
 *                               TW_EVENT_CAPTURE_START
 *
 *    Return Values:
 *      TW_RET_SUCCESS    - Successful in getting the event capture start/stop state of the 
 *                            specified controller
 *      TW_RET_FAILED     - Failed to get the event capture start/stop state of the specified 
 *                            controller.
 *
 *    Remarks:
 *      If the capture mode is TW_EVENT_CAPTURE_UNTIL_FULL, the controller will 
 *      automatically stop capturing as soon as the buffer is full.
 *
 *    See also:
 *      tw_enable_event_capture
 *      tw_start_event_capture
 *      tw_stop_event_capture
 *      tw_set_event_capture_mode
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_control_state(
      tw_controller_id_type           controller_id,
      tw_event_capture_control_type  *control_state
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides AEC host IO filter mask settings of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      host_io_mask        - The address where the tw_get_event_capture_host_io_mask will
 *                              store a 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The host IO filter mask for the specified controller
 *                              is available.
 *      TW_RET_FAILED       - The host IO filter mask for the specified controller
 *                              is not available. 
 *
 *    Remarks:
 *      Call this routine to read the host IO filter settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_set_event_capture_host_io_mask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_host_io_mask (
      tw_controller_id_type     controller_id,
      unsigned int *            host_io_mask
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets a AEC host IO filter mask for the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      host_io_mask        - A 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successfully set the host IO filter mask for the 
 *                              specified controller
 *      TW_RET_FAILED       - Failed to set the host IO filter mask for the 
 *                              specified controller
 *
 *    Remarks:
 *      Call this routine to set the host IO filter settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_get_event_capture_host_io_mask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_event_capture_host_io_mask (
      tw_controller_id_type     controller_id,
      unsigned int              host_io_mask
      );

/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides AEC internal IO filter mask settings of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      internal_io_mask        - The address where the tw_get_event_capture_internal_io_mask will
 *                              store a 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The internal IO filter mask for the specified controller
 *                              is available.
 *      TW_RET_FAILED       - The internal IO filter mask for the specified controller
 *                              is not available. 
 *
 *    Remarks:
 *      Call this routine to read the internal IO filter settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_set_event_capture_internal_io_mask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_internal_io_mask (
      tw_controller_id_type     controller_id,
      unsigned int *            internal_io_mask
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets a AEC internal IO filter mask for the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      internal_io_mask        - A 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successful in setting the internal IO filter mask for the 
 *                              specified controller
 *      TW_RET_FAILED       - Failed to set the internal IO filter mask for the 
 *                              specified controller
 *
 *    Remarks:
 *      Call this routine to set the internal IO filter settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_get_event_capture_internal_io_mask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_event_capture_internal_io_mask (
      tw_controller_id_type     controller_id,
      unsigned int              internal_io_mask
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Provides AEC internal IO un-mask settings of the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      internal_io_unmask        - The address where the tw_get_event_capture_internal_io_unmask will
 *                              store a 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - The internal IO un-mask for the specified controller
 *                              is available.
 *      TW_RET_FAILED       - The internal IO un-mask for the specified controller
 *                              is not available. 
 *
 *    Remarks:
 *      Call this routine to read the internal IO un-mask settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_set_event_capture_internal_io_unmask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_get_event_capture_internal_io_unmask (
      tw_controller_id_type     controller_id,
      unsigned int *            internal_io_unmask
      );


/**
 * <pre>
 *    Supported Controllers:
 *                                          9000 series
 *
 *    Description:
 *      Sets a AEC internal IO un-mask for the specified controller.
 *
 *    Parameters:
 *      controller_id       - ID of the controller in question.
 *      internal_io_mask        - A 32-bit value.
 *
 *    Return Values:
 *      TW_RET_SUCCESS      - Successful in setting the internal IO un-mask for the 
 *                              specified controller
 *      TW_RET_FAILED       - Failed to set the internal IO un-mask for the 
 *                              specified controller
 *
 *    Remarks:
 *      Call this routine to set the internal IO un-mask settings of event capture feature for 
 *      a particular controller.
 *
 *    See also:
 *      tw_get_event_capture_internal_io_unmask
 *      tw_api_defines.h
 *      tw_api_types.h
 * </pre>
 *
 */
int tw_set_event_capture_internal_io_unmask (
      tw_controller_id_type     controller_id,
      unsigned int              internal_io_unmask
      );


#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif  /* _TW_API_AVID_H */

