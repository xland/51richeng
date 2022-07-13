import type { Knex } from "knex";
let dbInstance: Knex;
let dbPath;
let path = require("path");
if (location.href.startsWith("http://localhost")) {
  dbPath = path.join(process.execPath, "../../../../src/common/db.db");
} else {
  //todo
  dbPath = path.join(process.execPath, "../../../../src/common/db.db");
}
let knex = require("knex");
export let db: Knex = knex({
  client: "better-sqlite3",
  connection: { filename: dbPath },
  useNullAsDefault: true,
});
