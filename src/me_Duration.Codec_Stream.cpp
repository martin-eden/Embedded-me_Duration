// Duration to text stream codec

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
*/

/*
  Output sample: "0 010 011 012"

  Note that we want three-digit numbers. Our TUint_2's are
  always printed as five-digit numbers. And one-digit number for
  kilo-seconds.

  To get padding we're going directly to [me_WriteInteger] and
  call it's Freetown's function.
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>
#include <me_BaseInterfaces.h>
#include <me_BaseTypesIo.h>
#include <me_WriteInteger.h>

using namespace me_Duration;

/*
  Print duration to console
*/
TBool me_Duration::Write(
  TDuration Duration,
  IOutputStream * OutputStream
)
{
  if (!DurationIsValid(Duration))
    return false;

  if (!me_WriteInteger::Freetown::Write_U4(Duration.KiloS, 1, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_WriteInteger::Freetown::Write_U4(Duration.S, 3, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_WriteInteger::Freetown::Write_U4(Duration.MilliS, 3, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_WriteInteger::Freetown::Write_U4(Duration.MicroS, 3, OutputStream))
    return false;

  return true;
}

/*
  Read duration from console
*/
TBool me_Duration::Read(
  TDuration * Duration,
  IInputStream * InputStream
)
{
  if (!me_BaseTypesIo::Read_Uint_2(&Duration->KiloS, InputStream))
    return false;
  if (!me_BaseTypesIo::Read_Uint_2(&Duration->S, InputStream))
    return false;
  if (!me_BaseTypesIo::Read_Uint_2(&Duration->MilliS, InputStream))
    return false;
  if (!me_BaseTypesIo::Read_Uint_2(&Duration->MicroS, InputStream))
    return false;

  if (!DurationIsValid(*Duration))
    return false;

  return true;
}

/*
  2025-12-26
*/
