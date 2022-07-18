import { ipcMain } from "electron";
import { DialogItem } from "./DialogItem";

class DialogPool {
  pool: DialogItem[] = [];
  private getBlankWindow(): DialogItem {
    for (let item of this.pool) {
      if (item.name === undefined) {
        return item;
      }
    }
    return this.createBlankWindow();
  }
  private getOpenDialog(viewName: string): DialogItem | null {
    for (let item of this.pool) {
      if (item.name === viewName) {
        return item;
      }
    }
    return null;
  }
  private showDialog(viewName) {
    let dialog = this.getOpenDialog(viewName);
    if (!dialog) {
      dialog = this.getBlankWindow();
      dialog.show(viewName);
      this.createBlankWindow();
    } else {
      dialog.dialog.show();
      dialog.dialog.flashFrame(true);
    }
  }
  private initListener() {
    ipcMain.handle("showDialog", (e, name) => this.showDialog(name));
  }
  private dialogClose(item: DialogItem) {
    let index = this.pool.findIndex((v) => v === item);
    if (index >= 0) this.pool.splice(index, 1);
  }
  private createBlankWindow() {
    let item = new DialogItem((item: DialogItem) => this.dialogClose(item));
    this.pool.push(item);
    return item;
  }
  init() {
    this.initListener();
    this.createBlankWindow();
  }
}
export let dialogPool = new DialogPool();
