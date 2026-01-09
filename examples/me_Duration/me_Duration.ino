// [me_Duration] test

/*
  Author: Martin Eden
  Last mod.: 2026-01-10
*/

#include <me_Duration.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_DebugPrints.h>
#include <me_StreamsCollection.h>

static void TestDurationToMicros(
  me_Duration::TDuration Duration
)
{
  TUint_4 Micros;

  me_DebugPrints::PrintDuration("Duration", Duration);

  if (!me_Duration::MicrosFromDuration(&Micros, Duration))
  {
    Console.Print("Conversion to micros failed");
    goto Done;
  }

  me_DebugPrints::Print("Micros", Micros);

Done:

  Console.EndLine();
}

static void TestMicrosToDuration(
  TUint_4 Micros
)
{
  me_Duration::TDuration Duration;

  me_DebugPrints::Print("Micros", Micros);

  if (!me_Duration::DurationFromMicros(&Duration, Micros))
  {
    Console.Print("Conversion from micros failed");
    goto Done;
  }

  me_DebugPrints::PrintDuration("Duration", Duration);

Done:

  Console.EndLine();
}

static void TestDurationsToMicros()
{
  Console.Print("( Durations to micros");
  Console.Indent();

  TestDurationToMicros({ 0, 0, 0, 0 });
  TestDurationToMicros({ 0, 0, 0, 1 });
  TestDurationToMicros({ 0, 0, 0, 999 });
  TestDurationToMicros({ 0, 0, 1, 0 });
  TestDurationToMicros({ 3, 999, 999, 999 });
  TestDurationToMicros({ 4, 0, 0, 0 });

  Console.Unindent();
  Console.Print(")");
}

static void TestMicrosToDurations()
{
  Console.Print("( Micros to durations");
  Console.Indent();

  TestMicrosToDuration(0);
  TestMicrosToDuration(1);
  TestMicrosToDuration(999);
  TestMicrosToDuration(1000);
  TestMicrosToDuration(3999999999);
  TestMicrosToDuration(4000000000);

  Console.Unindent();
  Console.Print(")");
}

/*
  Tour: Int -> Dur -> OutStream -> InStream -> Dur -> Int
*/
static void TestFullCycle(
  TUint_4 Micros
)
{
  TUint_4 OrigMicros;
  me_Duration::TDuration Duration;
  TUint_1 Buffer[40];
  TAddressSegment BufSeg = M_AsAddrSeg(Buffer);
  me_StreamsCollection::TWorkmemInputStream InStream;
  me_StreamsCollection::TWorkmemOutputStream OutStream;

  OrigMicros = Micros;

  Console.Print("(");
  Console.Indent();

  me_DebugPrints::Print("Micros", Micros);
  if (!me_Duration::DurationFromMicros(&Duration, Micros))
    Console.Print("Failed to get duration");
  Console.EndLine();

  OutStream.Init(BufSeg);
  me_Duration::Write(Duration, &OutStream);
  Console.Write("Duration (");
  Console.Write(OutStream.GetProcessedSegment());
  Console.Write(")");
  Console.EndLine();

  InStream.Init(OutStream.GetProcessedSegment());
  if (!me_Duration::Read(&Duration, &InStream))
    Console.Print("Failed to read duration");

  if (!me_Duration::MicrosFromDuration(&Micros, Duration))
    Console.Print("Failed to get micros");
  me_DebugPrints::Print("Micros", Micros);

  if (Micros != OrigMicros)
    Console.Print("Did not arrive to original value");

  Console.Unindent();
  Console.Print(")");
}

static void TestFullCycles()
{
  Console.Print("( Micros - Duration - OutStream - InStream - Duration - Micros");
  Console.Indent();

  TestFullCycle(10);
  TestFullCycle(10011);
  TestFullCycle(10011012);
  TestFullCycle(4011012013);

  Console.Unindent();
  Console.Print(")");
}

static void RunTests()
{
  TestDurationsToMicros();
  TestMicrosToDurations();
  TestFullCycles();
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
  2025-12-26
*/
