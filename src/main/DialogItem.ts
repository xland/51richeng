import { BrowserWindow } from "electron";
import { ConfigWindow } from "./ConfigWindow";

export class DialogItem {
  dialog: BrowserWindow;
  name: string;
  show(viewName: string) {
    this.dialog.webContents.send("initDialogView", viewName);
    this.name = viewName;
  }
  constructor(closeCb: Function) {
    let config = new ConfigWindow();
    this.dialog = new BrowserWindow(config);
    this.dialog.loadURL("http://localhost:3000/?viewName=dialog");
    this.dialog.webContents.openDevTools({ mode: "undocked" });
    this.dialog.on("close", () => closeCb(this));
  }
}
