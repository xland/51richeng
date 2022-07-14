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
  changeView(viewType) {
    let setting = get(this.store);
    setting.viewSelected = viewType;
    this.store.set(setting);
  }
}
export let settingStore = new SettingStore();
