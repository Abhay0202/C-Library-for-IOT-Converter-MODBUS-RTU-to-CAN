/*-----------------------------------------------------------------------------*/
/**
  @file  ql_rtc.h
  @brief   rtc API
*/
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  Copyright (c) 2018 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  EDIT HISTORY
  This section contains comments describing changes made to the file.
  Notice that changes are listed in reverse chronological order.

  when       	who       	what, where, why
  --------		---			---------------------------------------------------
  20181229   	Rambo.shan  Created
--------------------------------------------------------------------------------*/

#ifndef __QL_RTC_H__
#define __QL_RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Get the RTC time after verification
  @param[out] p_utc_msec The RTC time after verification storage for millisecond
  @return 
  0 - succeed 
  -1 - error ocurred
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_rtc_time_get(int64_t *p_utc_msec);

/*-----------------------------------------------------------------------------------------------*/
/**
  @brief Set the RTC time to correct the delta time of the RTC
  @param[in] utc_msec Current UTC time storage for millisecond
  @return 
  0 - succeed 
  -1 - error ocurred
  */
/*-----------------------------------------------------------------------------------------------*/
int ql_rtc_time_set(int64_t utc_msec);

#ifdef __cplusplus
}
#endif
#endif
