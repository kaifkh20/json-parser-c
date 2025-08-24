<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>JSON Parser in C — README</title>
  <style>
    :root {
      --bg: #0b0f14;
      --card: #0f1520;
      --text: #e6edf3;
      --muted: #9fb0c0;
      --accent: #7dc4ff;
      --code: #0b1220;
      --border: #1e2a3a;
      --green: #86efac;
    }
    html, body { margin: 0; padding: 0; background: var(--bg); color: var(--text); font-family: ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, Ubuntu, Cantarell, Noto Sans, Helvetica Neue, Arial, Apple Color Emoji, Segoe UI Emoji, Segoe UI Symbol; line-height: 1.6; }
    .wrap { max-width: 980px; margin: 0 auto; padding: 32px 20px 96px; }
    header { display: flex; align-items: center; justify-content: space-between; gap: 16px; padding: 20px; background: linear-gradient(180deg, #0f172a99, transparent); position: sticky; top: 0; backdrop-filter: blur(8px); border-bottom: 1px solid var(--border); }
    h1 { font-size: 1.9rem; margin: 0; letter-spacing: 0.2px; }
    .badge { border: 1px solid var(--border); padding: 6px 10px; border-radius: 999px; color: var(--muted); font-size: 0.9rem; }
    .card { background: var(--card); border: 1px solid var(--border); border-radius: 16px; padding: 20px; margin: 18px 0; }
    h2 { font-size: 1.35rem; margin: 0 0 10px; }
    h3 { font-size: 1.1rem; margin: 18px 0 10px; }
    code, pre { font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, Liberation Mono, monospace; }
    pre { background: var(--code); border: 1px solid var(--border); padding: 14px; border-radius: 12px; overflow-x: auto; }
    code.inline { background: var(--code); border: 1px solid var(--border); padding: 2px 6px; border-radius: 6px; }
    .toc a { color: var(--accent); text-decoration: none; }
    .toc a:hover { text-decoration: underline; }
    ul, ol { padding-left: 22px; }
    .grid { display: grid; gap: 12px; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); }
    .kicker { color: var(--muted); font-size: 0.95rem; margin-top: -6px; }
    .ok { color: var(--green); font-weight: 600; }
    .footer { color: var(--muted); font-size: 0.9rem; margin-top: 48px; }
  </style>
</head>
<body>
  <header>
    <h1>JSON Parser in C</h1>
    <span class="badge">Build: <span class="ok">make lin / make win</span> · Tests: <span class="ok">bun test</span></span>
  </header>

  <main class="wrap">
    <section class="card">
      <p>A lightweight, modular JSON parser written in <strong>C</strong>. It provides a clean API to tokenize, parse, and manage JSON objects, arrays, and values with safe memory handling.</p>
      <p class="kicker">This HTML file mirrors the README content in a copy‑pasteable, single‑file format.</p>
    </section>

    <section class="card toc">
      <h2>Table of Contents</h2>
      <ol>
        <li><a href="#project-structure">Project Structure</a></li>
        <li><a href="#features">Features</a></li>
        <li><a href="#build-instructions">Build Instructions</a></li>
        <li><a href="#running-tests">Running Tests</a></li>
        <li><a href="#usage-example">Usage Example</a></li>
        <li><a href="#future-improvements">Future Improvements</a></li>
        <li><a href="#license">License</a></li>
      </ol>
    </section>

    <section id="project-structure" class="card">
      <h2>Project Structure</h2>
      <pre><code>.
├── include/
│   ├── lexer.h         # Lexical analyzer (tokenizer)
│   ├── parser.h        # Core parser and JSON data structures
│   ├── utility_func.h  # Memory management + debugging helpers
│   └── json_parser.h   # High-level parsing interface
├── src/
│   ├── lexer.c
│   ├── parser.c
│   ├── utility_func.c
│   ├── json_parser.c
│   └── stack.c
├── main.c              # Example entry point / test runner
├── makefile            # Build script (Linux & Windows)
└── tests/              # JS tests (bun test)</code></pre>
    </section>

    <section id="features" class="card">
      <h2>Features</h2>
      <ul>
        <li>Tokenizes JSON strings into typed tokens.</li>
        <li>Parses into nested C data structures:
          <ul>
            <li><code class="inline">Object</code></li>
            <li><code class="inline">Array</code></li>
            <li><code class="inline">Value</code> (<code class="inline">STRING</code>, <code class="inline">INTEGER</code>, <code class="inline">FLOAT</code>, <code class="inline">BOOLEAN</code>, <code class="inline">NULL</code>, etc.)</li>
          </ul>
        </li>
        <li>Safe memory deallocation helpers.</li>
        <li>Simple API for parsing JSON from strings.</li>
      </ul>
    </section>

    <section id="build-instructions" class="card">
      <h2>Build Instructions</h2>
      <p>Controlled by the project <code class="inline">makefile</code>:</p>
      <h3>Linux</h3>
      <pre><code>make lin
# output: ./jp</code></pre>

      <h3>Windows (cross-compile)</h3>
      <pre><code>make win
# output: ./jp-w.exe  (requires MinGW)</code></pre>

      <h3>Clean</h3>
      <pre><code>make clean        # remove all builds
make clean-lin    # remove Linux build only
make clean-win    # remove Windows build only</code></pre>
    </section>

    <section id="running-tests" class="card">
      <h2>Running Tests</h2>
      <p>Tests are written with <a href="https://bun.sh/docs/test" target="_blank" rel="noreferrer noopener">bun test</a>.</p>
      <pre><code>bun test</code></pre>
      <p class="kicker">Ensure Bun is installed: <code class="inline">curl -fsSL https://bun.sh/install | bash</code></p>
    </section>

    <section id="usage-example" class="card">
      <h2>Usage Example</h2>
<pre><code>#include "json_parser.h"

int main() {
    char* json_input = "{ \"name\": \"Alice\", \"age\": 25, \"student\": false }";

    // Parse JSON string
    ResponseKV result = json_parser(json_input);

    // Work with result...
    // e.g. traverse values, print types, etc.

    // Free allocated memory
    free_mem(result);
    return 0;
}</code></pre>

      <h3>Compile &amp; Run</h3>
      <pre><code>make lin
./jp</code></pre>
    </section>

    <section id="future-improvements" class="card">
      <h2>Future Improvements</h2>
      <ul>
        <li>Better error reporting for malformed JSON.</li>
        <li>Support for streaming large JSON files.</li>
        <li>Helper functions for traversal/querying (like JSONPath).</li>
      </ul>
    </section>

    <section id="license" class="card">
      <h2>License</h2>
      <p>Add your preferred license (MIT, Apache-2.0, etc.).</p>
    </section>

    <p class="footer">© README generated for the C JSON Parser project. You can safely copy &amp; paste this file as your project README.html.</p>
  </main>
</body>
</html>
