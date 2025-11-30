// [me_Duration] test

/*
  Author: Martin Eden
  Last mod.: 2025-11-30
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>

using me_DebugPrints::PrintDuration;

void Compare(
  me_Duration::TDuration A,
  me_Duration::TDuration B
)
{
  PrintDuration(A);

  if (me_Duration::IsLess(A, B))
    Console.Write("<");

  if (me_Duration::IsGreater(A, B))
    Console.Write(">");

  if (me_Duration::AreEqual(A, B))
    Console.Write("=");

  PrintDuration(B);

  Console.EndLine();
}

void TestCompare()
{
  Console.Print("( Comparison");
  Console.Indent();

  Compare({ 000, 000, 000, 000 }, { 000, 000, 000, 000 });
  Compare({ 000, 999, 000, 000 }, { 000, 000, 000, 000 });
  Compare({ 000, 000, 000, 000 }, { 000, 000, 001, 000 });

  Console.Unindent();
  Console.Print(")");
}

void Add(
  me_Duration::TDuration A,
  me_Duration::TDuration B
)
{
  PrintDuration(A);

  Console.Write("+");

  PrintDuration(B);

  Console.Write("=");

  me_Duration::WrappedAdd(&A, B);

  PrintDuration(A);

  Console.EndLine();
}

void TestAdd()
{
  Console.Print("( Wrapped addition");
  Console.Indent();

  // A + B = B + A  -- commutativity
  Add({ 0, 0, 0, 1 }, { 0, 0, 0, 2 });
  Add({ 0, 0, 0, 2 }, { 0, 0, 0, 1 });

  // Carry
  Add({ 0, 0, 1, 0 }, { 0, 0, 999, 0 });

  // Overflow
  Add({ 500, 0, 0, 2 }, { 499, 999, 999, 999 });

  // Max field values
  Add({ 0, 0, 999, 0 }, { 0, 0, 999, 0 });

  // Invalid field value
  Add({ 0, 0, 0, 0 }, { 0, 0, 0, 1023 });

  Console.Unindent();
  Console.Print(")");
}

void Sub(
  me_Duration::TDuration A,
  me_Duration::TDuration B
)
{
  PrintDuration(A);

  Console.Write("-");

  PrintDuration(B);

  Console.Write("=");

  me_Duration::WrappedSub(&A, B);

  PrintDuration(A);

  Console.EndLine();
}

void TestSub()
{
  Console.Print("( Wrapped subtraction");
  Console.Indent();

  // Borrow
  Sub({ 0, 1, 0, 0 }, { 0, 0, 1, 0 });
  // Borrow
  Sub({ 0, 1, 0, 8 }, { 0, 0, 0, 9 });
  // Underflow
  Sub({ 0, 0, 1, 0 }, { 0, 0, 2, 0 });
  // Underflow
  Sub({ 499, 999, 999, 999 }, { 500, 0, 0, 2 });
  // Correct
  Sub({ 500, 0, 0, 2 }, { 499, 999, 999, 999 });

  Console.Unindent();
  Console.Print(")");
}

void TestDurationToMicros(
  me_Duration::TDuration Duration
)
{
  TUint_4 Micros;

  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);

  if (!me_Duration::DurationToMicros(&Micros, Duration))
    Console.Print("Conversion result capped");

  me_DebugPrints::Print("Micros", Micros);

  Console.EndLine();
}

void TestMicrosToDuration(
  TUint_4 Micros
)
{
  me_Duration::TDuration Duration;

  me_DebugPrints::Print("Micros", Micros);

  if (!me_Duration::MicrosToDuration(&Duration, Micros))
    Console.Print("Conversion result capped");

  Console.Write("Duration");
  me_DebugPrints::PrintDuration(Duration);

  Console.EndLine();
}

void TestDurationsToMicros()
{
  Console.Print("( Durations to micros");
  Console.Indent();

  TestDurationToMicros({ 0, 0, 0, 0 });
  TestDurationToMicros({ 0, 0, 0, 1 });
  TestDurationToMicros({ 0, 0, 0, 999 });
  TestDurationToMicros({ 0, 0, 1, 0 });
  TestDurationToMicros({ 0, 1, 0, 0 });
  TestDurationToMicros({ 1, 0, 0, 0 });
  TestDurationToMicros({ 4, 0, 0, 0 });
  TestDurationToMicros({ 4, 0, 0, 1 });

  Console.Unindent();
  Console.Print(")");
}

void TestMicrosToDurations()
{
  Console.Print("( Micros to durations");
  Console.Indent();

  TestMicrosToDuration(0);
  TestMicrosToDuration(1);
  TestMicrosToDuration(999);
  TestMicrosToDuration(1000);
  TestMicrosToDuration(1000000);
  TestMicrosToDuration(1000000000);
  TestMicrosToDuration(4000000000);
  TestMicrosToDuration(4000000001);

  Console.Unindent();
  Console.Print(")");
}

void RunTests()
{
  TestCompare();
  TestAdd();
  TestSub();
  TestDurationsToMicros();
  TestMicrosToDurations();
}

void setup()
{
  Console.Init();
  Console.Print("( [me_Duration] test");

  Console.Indent();
  RunTests();
  Console.Unindent();

  Console.Print(") Done");
}

void loop()
{
}

/*
  2025-03-02
  2025-08-01
  2025-09-19
  2025-11-30
*/
