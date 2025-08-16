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

// --- Helper to run parser with Valgrind ---
async function checkLeaks(filePath) {
  if (isWin) return; // skip memory tests on Windows

  return new Promise((resolve, reject) => {
    const proc = spawn("valgrind", [
      "--leak-check=full",
      "--errors-for-leak-kinds=definite",
      "--error-exitcode=1",
      exe,
      filePath,
      "--test",
    ]);

    let stderr = "";
    proc.stderr.on("data", (data) => (stderr += data.toString()));

    proc.on("close", (code) => {
      const leakMatch = stderr.match(/definitely lost: (\d+) bytes/);
      if (code !== 0 && leakMatch && parseInt(leakMatch[1]) > 0) {
        reject(new Error(`Valgrind detected definite leaks:\n${stderr}`));
      } else {
        resolve();
      }
    });
  });
}

// --- Memory Leak Tests ---
describe("JSON Parser Memory Checks", () => {
  const files = [
    "test/tests_file/test1.json",
    "test/tests_file/test2.json",
    "test/tests_file/test3.json",
    "test/tests_file/test4.json",
    "test/tests_file/test5.json",
    "test/tests_file/test6.json",
    "test/tests_file/test7.json",
    "test/tests_file/test8.json",
    "test/tests_file/test9.json",
    "test/tests_file/test10.json",
    "test/tests_file/test11.json",
    "test/tests_file/test12.json",
    "test/tests_file/test13.json",
  ];

  for (const file of files) {
    test(`Memory check: ${file}`, async () => {
      await checkLeaks(file);
    });
  }
});

// --- Clean after tests ---
afterAll(async () => {
  const target = isWin ? "clean-win" : "clean-lin";
  const proc = Bun.spawn(["make", target]);
  await proc.exited;
});

