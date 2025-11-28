// Time interval definition and management

/*
  Author: Martin Eden
  Last mod.: 2025-11-28
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Duration
{
  /*
    Duration record

    Size: 8 bytes
    Capacity: 1 mega-second (~11 days)
    Granularity: 1 micro-second

    Contract

      * Unit stores integer values between 0 and 999
      * Next unit is 1000 times less

    Original structure used 10-bit fields and occupied 5 bytes.
    It worked fine and you can revert back to it.

    But there are more CPU operations to get/set field in this case.
    It means minimum time to access field is higher when you're using
    bitfields.
  */
  struct TDuration
  {
    TUint_2 KiloS;
    TUint_2 S;
    TUint_2 MilliS;
    TUint_2 MicroS;
  };

  // Minimum value
  const TDuration MinValue = { 0, 0, 0, 0 };

  // Maximum value
  const TDuration MaxValue = { 999, 999, 999, 999 };

  // Get/set "volatile" record
  TDuration GetVolatile(volatile TDuration&);
  void SetVolatile(volatile TDuration&, TDuration);

  /*
    Comparison

    (
      AreEqual
      IsLess
      IsGreater
      IsLessOrEqual
      IsGreaterOrEqual
    ) - boolean functions for readable code
  */
  TBool AreEqual(TDuration, TDuration);
  TBool IsLess(TDuration, TDuration);
  TBool IsGreater(TDuration, TDuration);
  TBool IsLessOrEqual(TDuration, TDuration);
  TBool IsGreaterOrEqual(TDuration, TDuration);

  /*
    Add/subtract functions
  */
  void WrappedAdd(TDuration *, TDuration);
  void WrappedSub(TDuration *, TDuration);
  void CappedAdd(TDuration *, TDuration);
  void CappedSub(TDuration *, TDuration);

  /*
    Import/export
  */
  TUint_4 DurationToMicros(TDuration);
  TDuration MicrosToDuration(TUint_4);
}

/*
  2025-03-02
  2025-08-02
  2025-09-15
  2025-09-20 Renamed from Timestamp to Duration
  2025-10-30 Duration to micros conversion
  2025-11-25 No bitfields in record
  2025-11-28 "Capped" and "Wrapped" flavors of Add/Sub
*/
