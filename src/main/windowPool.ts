import { WindowItem } from "./WindowItem";

class WindowPool {
  pool: WindowItem[] = [];
  init() {
    let item = new WindowItem();
    this.pool.push(item);
  }
}
export let windowPool = new WindowPool();
