import { defineConfig } from "vite";
import { svelte } from "@sveltejs/vite-plugin-svelte";
import optimizer from "vite-plugin-optimizer";

let getReplacer = () => {
  let externalModels = ["os", "fs", "path", "buffer", "events", "child_process", "crypto", "http", "buffer", "url", "better-sqlite3", "knex"];
  let result = {};
  for (let item of externalModels) {
    result[item] = () => ({
      find: new RegExp(`^${item}$`),
      code: `const ${item} = require('${item}');export { ${item} as default }`,
    });
  }
  result["electron"] = () => ({
    find: new RegExp(`^electron$`),
    code: `const { ipcRenderer } = require('electron'); export { ipcRenderer };`,
  });
  return result;
};
export default defineConfig({
  plugins: [optimizer(getReplacer()), svelte()],
});

//https://github.com/vite-plugin/vite-plugin-optimizer/blob/main/README.zh-CN.md
