import type { SettingModel } from "../model/SettingModel";
import { db } from "./db";

class GlobalSetting {
  private instance: SettingModel;
  async getSetting() {
    if (this.instance) return this.instance;
    this.instance = await db("Setting").first();
    return this.instance;
  }
}
export let globalSetting = new GlobalSetting();
