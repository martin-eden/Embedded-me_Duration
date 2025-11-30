// Time interval management

/*
  Author: Martin Eden
  Last mod.: 2025-11-30
*/

/*
  It's sort of "long arithmetic": (0 0 123 456) means 123 milli-seconds
  and 456 micro-seconds.
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>

#include <avr/common.h> // SREG
#include <avr/interrupt.h> // cli()

using namespace me_Duration;

const TUint_2 MaxFieldValue = 999;

/*
  Return copy of volatile record

  I hate C.
*/
TDuration me_Duration::GetVolatile(
  volatile TDuration &A
)
{
  me_Duration::TDuration Result;
  TUint_1 PrevSreg;

  PrevSreg = SREG;
  cli();

  Result.KiloS = A.KiloS;
  Result.S = A.S;
  Result.MilliS = A.MilliS;
  Result.MicroS = A.MicroS;

  SREG = PrevSreg;

  return Result;
}

/*
  Set volatile record
*/
void me_Duration::SetVolatile(
  volatile TDuration &A,
  TDuration B
)
{
  TUint_1 PrevSreg;

  PrevSreg = SREG;
  cli();

  A.KiloS = B.KiloS;
  A.S = B.S;
  A.MilliS = B.MilliS;
  A.MicroS = B.MicroS;

  SREG = PrevSreg;
}

/*
  True when timestamps are equal
*/
TBool me_Duration::AreEqual(
  TDuration A,
  TDuration B
)
{
  return
    (A.KiloS == B.KiloS) &&
    (A.S == B.S) &&
    (A.MilliS == B.MilliS) &&
    (A.MicroS == B.MicroS);
}

/*
  True when timestamp is less
*/
TBool me_Duration::IsLess(
  TDuration A,
  TDuration B
)
{
  if (A.KiloS < B.KiloS)
    return true;
  if (A.KiloS > B.KiloS)
    return false;

  if (A.S < B.S)
    return true;
  if (A.S > B.S)
    return false;

  if (A.MilliS < B.MilliS)
    return true;
  if (A.MilliS > B.MilliS)
    return false;

  if (A.MicroS < B.MicroS)
    return true;

  return false;
}

/*
  True when timestamp is greater
*/
TBool me_Duration::IsGreater(
  TDuration A,
  TDuration B
)
{
  return IsLess(B, A);
}

/*
  True when timestamp is less or equal
*/
TBool me_Duration::IsLessOrEqual(
  TDuration A,
  TDuration B
)
{
  return
    IsLess(A, B) ||
    AreEqual(A, B);
}

/*
  True when timestamp is greater or equal
*/
TBool me_Duration::IsGreaterOrEqual(
  TDuration A,
  TDuration B
)
{
  return
    IsGreater(A, B) ||
    AreEqual(A, B);
}

/*
  Check that duration data is valid

  All fields must lie in [000, 999].
*/
TBool DurationIsValid(
  TDuration Dur
)
{
  return
    (Dur.KiloS <= MaxFieldValue) &&
    (Dur.S <= MaxFieldValue) &&
    (Dur.MilliS <= MaxFieldValue) &&
    (Dur.MicroS <= MaxFieldValue);
}

// [Internal] Add with wrapping. Returns false on overflow
static TBool Add(
  TDuration * A,
  TDuration B
)
{
  TUint_2 KiloS, S, MilliS, MicroS;
  TUint_1 Carry, NextCarry;

  KiloS = A->KiloS;
  S = A->S;
  MilliS = A->MilliS;
  MicroS = A->MicroS;

  Carry = 0;

  MicroS = MicroS + B.MicroS + Carry;
  NextCarry = (MicroS > MaxFieldValue);
  MicroS = MicroS % (MaxFieldValue + 1);

  Carry = NextCarry;

  MilliS = MilliS + B.MilliS + Carry;
  NextCarry = (MilliS > MaxFieldValue);
  MilliS = MilliS % (MaxFieldValue + 1);

  Carry = NextCarry;

  S = S + B.S + Carry;
  NextCarry = (S > MaxFieldValue);
  S = S % (MaxFieldValue + 1);

  Carry = NextCarry;

  KiloS = KiloS + B.KiloS + Carry;
  NextCarry = (KiloS > MaxFieldValue);
  KiloS = KiloS % (MaxFieldValue + 1);

  Carry = NextCarry;

  A->KiloS = KiloS;
  A->S = S;
  A->MilliS = MilliS;
  A->MicroS = MicroS;

  return (Carry == 0);
}

// [Internal] Subtract with wrapping. Returns false on underflow
static TBool Sub(
  TDuration * A,
  TDuration B
)
{
  TUint_2 KiloS, S, MilliS, MicroS;
  TUint_1 Borrow, NextBorrow;

  KiloS = A->KiloS;
  S = A->S;
  MilliS = A->MilliS;
  MicroS = A->MicroS;

  Borrow = 0;

  NextBorrow = (MicroS < (TUint_2) B.MicroS + Borrow);
  MicroS = MicroS + (MaxFieldValue + 1) - B.MicroS - Borrow;
  MicroS = MicroS % (MaxFieldValue + 1);

  Borrow = NextBorrow;

  NextBorrow = (MilliS < (TUint_2) B.MilliS + Borrow);
  MilliS = MilliS + (MaxFieldValue + 1) - B.MilliS - Borrow;
  MilliS = MilliS % (MaxFieldValue + 1);

  Borrow = NextBorrow;

  NextBorrow = (S < (TUint_2) B.S + Borrow);
  S = S + (MaxFieldValue + 1) - B.S - Borrow;
  S = S % (MaxFieldValue + 1);

  Borrow = NextBorrow;

  NextBorrow = (KiloS < (TUint_2) B.KiloS + Borrow);
  KiloS = KiloS + (MaxFieldValue + 1) - B.KiloS - Borrow;
  KiloS = KiloS % (MaxFieldValue + 1);

  Borrow = NextBorrow;

  A->KiloS = KiloS;
  A->S = S;
  A->MilliS = MilliS;
  A->MicroS = MicroS;

  return (Borrow == 0);
}

/*
  Add timestamp with wrapping
*/
void me_Duration::WrappedAdd(
  TDuration * A,
  TDuration B
)
{
  if (!DurationIsValid(*A)) return;
  if (!DurationIsValid(B)) return;

  Add(A, B);
}

/*
  Capped add
*/
void me_Duration::CappedAdd(
  TDuration * A,
  TDuration B
)
{
  if (!DurationIsValid(*A)) return;
  if (!DurationIsValid(B)) return;

  if (!Add(A, B))
    *A = MaxValue;
}

/*
  Subtract duration with wrapping
*/
void me_Duration::WrappedSub(
  TDuration * A,
  TDuration B
)
{
  if (!DurationIsValid(*A)) return;
  if (!DurationIsValid(B)) return;

  Sub(A, B);
}

/*
  Capped subtract
*/
void me_Duration::CappedSub(
  TDuration * A,
  TDuration B
)
{
  if (!DurationIsValid(*A)) return;
  if (!DurationIsValid(B)) return;

  if (!Sub(A, B))
    *A = MinValue;
}

const TUint_4 MaxMicrosToConvert = 4000000000;
const TDuration MaxDurationToConvert = { 4, 0, 0, 0 };

/*
  Convert duration record to number of microseconds
*/
TBool me_Duration::DurationToMicros(
  TUint_4 * NumMicros,
  me_Duration::TDuration Duration
)
{
  TBool IsCapped;

  IsCapped = false;

  if (IsGreater(Duration, MaxDurationToConvert))
  {
    Duration = MaxDurationToConvert;

    IsCapped = true;
  }

  *NumMicros =
    (TUint_4)
    1000000000 * Duration.KiloS +
    1000000 * Duration.S +
    1000 * Duration.MilliS +
    Duration.MicroS;

  return !IsCapped;
}

/*
  Convert number of microseconds to duration record
*/
TBool me_Duration::MicrosToDuration(
  me_Duration::TDuration * Duration,
  TUint_4 NumMicros
)
{
  TBool IsCapped;

  IsCapped = false;

  if (NumMicros > MaxMicrosToConvert)
  {
    NumMicros = MaxMicrosToConvert;

    IsCapped = true;
  }

  Duration->KiloS = (NumMicros / 1000000000) % 1000;
  Duration->S = (NumMicros / 1000000) % 1000;
  Duration->MilliS = (NumMicros / 1000) % 1000;
  Duration->MicroS = (NumMicros / 1) % 1000;

  return !IsCapped;
}

/*
  2025-03-02
  2025-03-03
  2025-08-01
  2025-10-28
  2025-10-30
  2025-11-28
*/
