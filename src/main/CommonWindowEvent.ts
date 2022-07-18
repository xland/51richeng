import { BrowserWindow, ipcMain, app, screen } from "electron";
import electget from "electget";

export class CommonWindowEvent {
  /**
   * 根据事件对象内的webContents属性获取对应的窗口
   * @param event 事件对象
   * @returns 窗口对象
   */
  private static getWin(event: any) {
    return BrowserWindow.fromWebContents(event.sender);
  }
  /**
   * 监听与窗口相关的事件
   * 使用事件对象获取对应的窗口，然后再对应的窗口上执行操作
   */
  public static listen() {
    ipcMain.handle("minimizeWindow", (e) => {
      this.getWin(e)?.minimize();
    });

    ipcMain.handle("maxmizeWindow", (e) => {
      this.getWin(e)?.maximize();
    });

    ipcMain.handle("unmaximizeWindow", (e) => {
      this.getWin(e)?.unmaximize();
    });

    ipcMain.handle("hideWindow", (e) => {
      this.getWin(e)?.hide();
    });
    ipcMain.handle("pinWindow", (e) => {
      let win = this.getWin(e) as BrowserWindow;
      win.show();
      let display = screen.getPrimaryDisplay();
      let x = display.workAreaSize.width - 808;
      let y = 8;
      win.setPosition(x, y);
      win.setSize(800, 600);
      win.setHasShadow(false); //todo 没用
      // electget.alwaysOnBottom(win); //todo issue
      electget.preventFromAeroPeek(win);
      electget.preventFromShowDesktop(win);
      electget.moveToBottom(win);
    });
    ipcMain.handle("showWindow", (e) => {
      this.getWin(e)?.show();
    });

    ipcMain.handle("closeWindow", (e) => {
      this.getWin(e)?.close();
    });
    ipcMain.handle("resizable", (e) => {
      return this.getWin(e)?.isResizable();
    });
    ipcMain.handle("sizeWindow", (e, param: { w: number; h: number; minW: number; minH: number }) => {
      let win = this.getWin(e);
      win.setSize(param.w, param.h);
      win.setMinimumSize(param.minW, param.minH);
      win.center();
      win.show();
    });
    ipcMain.handle("hasCookie", async (e, filter) => {
      let win = this.getWin(e);
      let cookies = await win.webContents.session.cookies.get(filter);
      return cookies;
    });
    ipcMain.handle("getPath", (e, name: any) => {
      return app.getPath(name);
    });
  }
}
