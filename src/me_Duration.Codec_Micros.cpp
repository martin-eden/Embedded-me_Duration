// Duration to microseconds codec

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>

using namespace me_Duration;

/*
  Convert duration record to number of microseconds
*/
TBool me_Duration::MicrosFromDuration(
  TUint_4 * NumMicros,
  me_Duration::TDuration Duration
)
{
  if (!DurationIsValid(Duration))
    return false;

  *NumMicros =
    1000000000L* Duration.KiloS +
    1000000L * Duration.S +
    1000L * Duration.MilliS +
    Duration.MicroS;

  return true;
}

/*
  Convert number of microseconds to duration record
*/
TBool me_Duration::DurationFromMicros(
  me_Duration::TDuration * Duration,
  TUint_4 NumMicros
)
{
  Duration->KiloS = (NumMicros / 1000000000) % 1000;
  Duration->S = (NumMicros / 1000000) % 1000;
  Duration->MilliS = (NumMicros / 1000) % 1000;
  Duration->MicroS = (NumMicros / 1) % 1000;

  if (!DurationIsValid(*Duration))
    return false;

  return true;
}

/*
  2025-10-30
  2025-12-26
*/
