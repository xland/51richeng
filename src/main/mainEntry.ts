import { app, BrowserWindow, protocol, ProtocolResponse, session } from "electron";
import electget from "electget";
import { CommonWindowEvent } from "./CommonWindowEvent";
import { ConfigWindow } from "./ConfigWindow";
process.env.ELECTRON_DISABLE_SECURITY_WARNINGS = "true";
app.commandLine.appendSwitch("--disable-site-isolation-trials");
let schemeConfig = { standard: true, supportFetchAPI: true, bypassCSP: true, corsEnabled: true, stream: true };
protocol.registerSchemesAsPrivileged([{ scheme: "rrs", privileges: schemeConfig }]);

let mainWindow: BrowserWindow;

app.whenReady().then(() => {
  let config = new ConfigWindow();
  config.minHeight = 600;
  config.minWidth = 800;
  mainWindow = new BrowserWindow(config);
  mainWindow.loadURL("http://localhost:3000/");
  mainWindow.webContents.openDevTools();
  CommonWindowEvent.listen();
  app.on("browser-window-focus", (e, win: BrowserWindow) => {
    if (win.id != mainWindow.id) return;
    electget.moveToBottom(mainWindow);
  });
  mainWindow.webContents.setWindowOpenHandler((param) => {
    console.log(param.url);
    let config = new ConfigWindow();
    config.momodalable = true;
    config.modal = true;
    config.parent = mainWindow;
    return { action: "allow", overrideBrowserWindowOptions: config };
  });
  // mainWindow.once("show", () => {
  //   dialogPool.init();
  // });
  // protocol.registerHttpProtocol("rrs", async (request, callback) => {
  //   if (request.method === "POST") {
  //     for (let i = 0; i < request.uploadData.length; i++) {
  //       console.log(request.uploadData[i]);
  //       if (request.uploadData[i].blobUUID) {
  //         let data = await session.defaultSession.getBlobData(request.uploadData[i].blobUUID);
  //         console.log(data);
  //       }
  //     }
  //   }
  // });
});
