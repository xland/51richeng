import typescript from "@rollup/plugin-typescript";
import { terser } from "rollup-plugin-terser";

const external = ["electron", "os", "fs", "path", "events", "child_process", "crypto", "http", "buffer", "url", "better-sqlite3", "knex"];
function startElectron() {
  let electronProcess = require("child_process").spawn(require("electron").toString(), ["./public/mainEntry.cjs"], {
    cwd: process.cwd(),
  });
  electronProcess.on("close", () => {
    electronProcess.kill(0);
    process.exit();
  });
  electronProcess.stdout.on("error", (data) => {
    data = data.toString();
    console.log(data);
  });
  electronProcess.stdout.on("data", (data) => {
    data = data.toString();
    console.log(data);
  });
}
function serve() {
  return {
    writeBundle() {
      startElectron();
    },
  };
}
export default {
  input: "src/main/mainEntry.ts",
  output: {
    sourcemap: false,
    format: "cjs",
    name: "app",
    file: "public/mainEntry.cjs",
  },
  plugins: [
    typescript({
      sourceMap: false,
      inlineSources: false,
    }),
    terser(),
    serve(),
  ],
  external,
};
