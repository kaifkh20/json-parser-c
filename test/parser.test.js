import { expect, test, beforeAll, afterAll, describe } from "bun:test";
import { spawn } from "child_process";
import { platform } from "node:os";

const isWin = platform() === "win32";
const exe = isWin ? "./jp-win.exe" : "./jp";

// --- Build before tests ---
beforeAll(async () => {
  const target = isWin ? "win" : "lin";
  const proc = Bun.spawn(["make", target]);
  await proc.exited;
});

// --- Helper to run parser and capture stdout/stderr ---
async function runParser(filePath) {
  return new Promise((resolve, reject) => {
    const proc = spawn(exe, [filePath, "--test"]);
    let stdout = "";
    let stderr = "";

    proc.stdout.on("data", (data) => (stdout += data.toString()));
    proc.stderr.on("data", (data) => (stderr += data.toString()));

    proc.on("close", (code) => {
      resolve({
        code,
        stdout: stdout.trim(),
        stderr: stderr.trim(),
        combined: (stdout + stderr).trim(),
      });
    });
  });
}

// --- Tests ---
describe("JSON Parser Output", () => {
  // Normal parsing tests (expected successful output)
  test("Valid Empty JSON", async () => {
    const { combined } = await runParser("test/tests_file/test1.json");
    expect(combined).toEqual("Succesfully Parsed");
  });

  test("Single key-value pair", async () => {
    const { combined } = await runParser("test/tests_file/test2.json");
    expect(combined).toEqual("Succesfully Parsed\nkey:value");
  });

  test("Multiple key-value pairs", async () => {
    const { combined } = await runParser("test/tests_file/test3.json");
    expect(combined).toEqual("Succesfully Parsed\nkey1:value1\nkey2:value2\nkey$:value[];");
  });

  test("Object as values", async () => {
    const { combined } = await runParser("test/tests_file/test6.json");
    expect(combined).toEqual("Succesfully Parsed\nkey1:<Object { key2_obj: value2_obj }\n>");
  });

  test("Integer as values", async () => {
    const { combined } = await runParser("test/tests_file/test7.json");
    expect(combined).toEqual("Succesfully Parsed\nkey:1");
  });
  
  test("Boolean as values", async () => {
    const { combined } = await runParser("test/tests_file/test14.json");
    expect(combined).toEqual("Succesfully Parsed\nkey:true");
  });


  test("Array as values", async () => {
    const { combined } = await runParser("test/tests_file/test8.json");
    expect(combined).toEqual("Succesfully Parsed\nkey:<Array valuearray>");
  });

  test("Nested arrays", async () => {
    const { combined } = await runParser("test/tests_file/test10.json");
    expect(combined).toEqual("Succesfully Parsed\nkey:<Array <Array hi>>");
  });

  test("Array as base value", async () => {
    const { combined } = await runParser("test/tests_file/test11.json");
    expect(combined).toEqual("Succesfully Parsed\n<Array hi>");
  });

  test("Null value", async () => {
    const { combined } = await runParser("test/tests_file/test13.json");
    expect(combined).toEqual("Succesfully Parsed\n<Array null>");
  });

  // Error parsing tests (expected error messages)
  test("Error: invalid opening tags", async () => {
    const { stderr } = await runParser("test/tests_file/test12.json");
    expect(stderr).toEqual("Parser Error: Invalid Opening Tags\nOperation Aborted");
  });

  test("Error: invalid syntax", async () => {
    const { stderr } = await runParser("test/tests_file/test4.json");
    expect(stderr).toEqual("Parser Error : Invalid Syntax\nOperation Aborted");
  });

  test("Error: lexical error", async () => {
    const { stderr } = await runParser("test/tests_file/test5.json");
    expect(stderr).toEqual("Lexical Error: $ not valid");
  });
});

// --- Clean after tests ---
afterAll(async () => {
  const target = isWin ? "clean-win" : "clean-lin";
  const proc = Bun.spawn(["make", target]);
  await proc.exited;
});
