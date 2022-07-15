import { BrowserWindow } from "electron";
import { ConfigWindow } from "./ConfigWindow";

export class WindowItem {
  win: BrowserWindow;
  constructor() {
    let config = new ConfigWindow();
    this.win = new BrowserWindow(config);
  }
}
