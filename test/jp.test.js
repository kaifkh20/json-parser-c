import { expect, test, beforeAll, afterAll, describe } from "bun:test";
import { platform } from "node:os";

const isWin = platform() === "win32";
const exe = isWin ? "./jp-win.exe" : "./jp";

// Build before tests
beforeAll(async () => {
  const target = isWin ? "win" : "lin";
  const proc = Bun.spawn(["make", target]);
  const code = await proc.exited;
  if (code !== 0) {
    throw new Error(`Build failed with exit code ${code}`);
  }
});

async function runScript(filePath) {
    let run_ex = "./jp"
    if (platform() === "win32") {
        run_ex = "./jp-win.exe"
    }

    const proc = Bun.spawn([run_ex, filePath, "--test"])
    const output = await new Response(proc.stdout).text()

    await proc.exited
    return output
}

// --- Tests ---
describe("JSON Parser", () => {
  test("Valid Empty JSON", async () => {
    const out = await runScript("test/tests_file/test1.json");
    expect(out).toEqual("Succesfully Parsed\n");
  });

  test("Single key-value pair", async () => {
    const out = await runScript("test/tests_file/test2.json");
    expect(out).toEqual("Succesfully Parsed\nkey:value\n");
  });

  test("Multiple key-value pairs", async () => {
    const out = await runScript("test/tests_file/test3.json");
    expect(out).toEqual("Succesfully Parsed\nkey1:value1\nkey2:value2\nkey$:value[];\n");
  });

  test("Object as values", async () => {
    const out = await runScript("test/tests_file/test6.json");
    expect(out).toEqual("Succesfully Parsed\nkey1:<Object>\n");
  });

  test("Integer as values", async () => {
    const out = await runScript("test/tests_file/test7.json");
    expect(out).toEqual("Succesfully Parsed\nkey:1\n");
  });

  test("Array as values", async () => {
    const out = await runScript("test/tests_file/test8.json");
    expect(out).toEqual("Succesfully Parsed\nkey:<Array valuearray>\n");
  });

  test("Object values inside array", async () => {
    const out = await runScript("test/tests_file/test9.json");
    expect(out).toEqual("Succesfully Parsed\nkey:<Array <Object>>\n");
  });

  test("Nested arrays", async () => {
    const out = await runScript("test/tests_file/test10.json");
    expect(out).toEqual("Succesfully Parsed\nkey:<Array <Array hi>>\n");
  });

  test("Array as base value", async () => {
    const out = await runScript("test/tests_file/test11.json");
    expect(out).toEqual("Succesfully Parsed\n<Array hi>\n");
  });

  test("Null value", async () => {
    const out = await runScript("test/tests_file/test13.json");
    expect(out).toEqual("Succesfully Parsed\n<Array null>\n");
  });

  test("Error: invalid opening tags", async () => {
    const out = await runScript("test/tests_file/test12.json");
    expect(out).toEqual("Parser Error: Invalid Opening Tags\nOperation Aborted\n");
  });

  test("Error: invalid syntax", async () => {
    const out = await runScript("test/tests_file/test4.json");
    expect(out).toEqual("Parser Error : Invalid Syntax\nOperation Aborted\n");
  });

  test("Error: lexical error", async () => {
    const out = await runScript("test/tests_file/test5.json");
    expect(out).toEqual("Lexical Error: $ not valid\n");
  });
});

// Clean after tests
afterAll(async () => {
  const target = isWin ? "clean-win" : "clean-lin";
  const proc = Bun.spawn(["make", target]);
  await proc.exited;
});
