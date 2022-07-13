import { get, Writable, writable } from "svelte/store";
import { db } from "../../common/db";
import type { SettingModel } from "../../model/SettingModel";
class SettingStore {
  store: Writable<SettingModel>;
  subscribe;
  async init() {
    let setting = await db("Setting").first();
    this.store = writable<SettingModel>(setting);
    this.subscribe = this.store.subscribe;
  }
  updateLeftBoxWidth = async (leftBoxWidth: number) => {
    let setting = get(this.store);
    setting.leftBoxWidth = leftBoxWidth;
    await db("setting").update({ leftBoxWidth }).where({ id: setting.id });
    this.store.set(setting);
  };
  updateMaximizeState = async (windowIsMaximized: boolean, windowWidth: number, windowHeight: number) => {
    let setting = get(this.store);
    setting.windowIsMaximized = windowIsMaximized;
    await db("setting").update({ windowIsMaximized, windowWidth, windowHeight }).where({ id: setting.id });
    this.store.set(setting);
  };
}
export let settingStore = new SettingStore();
