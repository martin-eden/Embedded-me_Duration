// Time interval definition and management

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
*/

/*
  Point of this module is "Duration" time record

  It is designed around "second" time unit and 10^3 scaling.
*/

/*
  All calculations over this type that was here before are removed

  Use TUint_4 for that. Here we are providing codecs and that's all.
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_BaseInterfaces.h>

namespace me_Duration
{
  /*
    Duration record

    Size: 8 bytes
    Capacity: 4 kilo-seconds (~1 hour)
    Granularity: 1 micro-second

    Contract

      * Unit stores integer values between 0 and 999
      * Next unit is 1000 times less

    Note

      Theoretical capacity is 1 mega-second (~11 days).
      But practical operations work with TUint_4.
      So validity check requires record to be under 4 kilo-seconds.
  */
  struct TDuration
  {
    TUint_2 KiloS;
    TUint_2 S;
    TUint_2 MilliS;
    TUint_2 MicroS;
  };

  // Validity check
  TBool DurationIsValid(TDuration);

  /*
    Import/export
  */
  // ( From/to microseconds
  TBool MicrosFromDuration(TUint_4 *, TDuration);
  TBool DurationFromMicros(TDuration *, TUint_4);
  // )
  // ( Console codec
  TBool Write(TDuration, IOutputStream *);
  TBool Read(TDuration *, IInputStream *);
  // )
}

/*
  2025-03-02
  2025-08-02
  2025-09-15
  2025-09-20 Renamed from Timestamp to Duration
  2025-10-30 Duration to micros conversion
  2025-11-25 No bitfields in record
  2025-11-28 "Capped" and "Wrapped" flavors of Add/Sub
  2025-12-26 Removed most code. Main point is text codec now.
*/
