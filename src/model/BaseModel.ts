export class BaseModel {
  id: string;
  getData() {
    let result: any = {};
    for (let key in this) {
      if (key.startsWith("_")) continue;
      result[key] = this[key];
    }
    return result;
  }
  constructor() {
    let crypto = require("crypto");
    this.id = crypto.randomUUID();
  }
}
