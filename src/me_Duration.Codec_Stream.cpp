// Duration to text stream codec

/*
  Author: Martin Eden
  Last mod.: 2025-12-26
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>
#include <me_BaseInterfaces.h>
#include <me_BaseTypesIo.h>

using namespace me_Duration;

/*
  Print duration to console
*/
TBool me_Duration::Print(
  TDuration Duration,
  IOutputStream * OutputStream
)
{
  if (!DurationIsValid(Duration))
    return false;

  if (!me_BaseTypesIo::Write_Uint_2(Duration.KiloS, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_BaseTypesIo::Write_Uint_2(Duration.S, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_BaseTypesIo::Write_Uint_2(Duration.MilliS, OutputStream))
    return false;
  if (!OutputStream->Write(' '))
    return false;
  if (!me_BaseTypesIo::Write_Uint_2(Duration.MicroS, OutputStream))
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
