// Time interval definition and management

/*
  Author: Martin Eden
  Last mod.: 2025-11-21
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Duration
{
  /*
    Duration record

    Size: 40 bits (5 bytes)
    Capacity: 1 mega-second (~11 days)
    Granularity: 1 micro-second
  */
  struct TDuration
  {
    TUint_2 KiloS : 10;
    TUint_2 S : 10;
    TUint_2 MilliS : 10;
    TUint_2 MicroS : 10;
  };

  // Zero constant
  const TDuration Zero = { 0, 0, 0, 0 };
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
  TBool AreEqual(TDuration A, TDuration B);
  TBool IsLess(TDuration A, TDuration B);
  TBool IsGreater(TDuration A, TDuration B);
  TBool IsLessOrEqual(TDuration A, TDuration B);
  TBool IsGreaterOrEqual(TDuration A, TDuration B);

  /*
    Add/subtract functions
  */
  TBool Add(TDuration * A, TDuration B);
  TBool Subtract(TDuration * A, TDuration B);

  /*
    Import/export
  */
  TUint_4 DurationToMicros(TDuration Duration);
  TDuration MicrosToDuration(TUint_4 NumMicros);
}

/*
  2025-03-02
  2025-08-02
  2025-09-15
  2025-09-20 Renamed from Timestamp to Duration
  2025-10-30 Duration to micros conversion
*/
