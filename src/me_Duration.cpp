// Duration common core

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>

using namespace me_Duration;

/*
  Check that duration data is valid

  All fields must lie in [000, 999] and
  duration should be less than 4 Ks.
*/
TBool me_Duration::DurationIsValid(
  TDuration Dur
)
{
  const TUint_2 MaxKilosValue = 3;
  const TUint_2 MaxNonKilosValue = 999;

  return
    (Dur.KiloS <= MaxKilosValue) &&
    (Dur.S <= MaxNonKilosValue) &&
    (Dur.MilliS <= MaxNonKilosValue) &&
    (Dur.MicroS <= MaxNonKilosValue);
}

/*
  2025-03-02
  2025-03-03
  2025-08-01
  2025-10-28
  2025-10-30
  2025-11-28
*/
