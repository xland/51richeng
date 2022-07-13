import { app, BrowserWindow, protocol, ProtocolResponse, session } from "electron";
import { CommonWindowEvent } from "./CommonWindowEvent";
import { ConfigWindow } from "./ConfigWindow";
// import busboy from "busboy";
process.env.ELECTRON_DISABLE_SECURITY_WARNINGS = "true";
app.commandLine.appendSwitch("--disable-site-isolation-trials");
let schemeConfig = { standard: true, supportFetchAPI: true, bypassCSP: true, corsEnabled: true, stream: true };
protocol.registerSchemesAsPrivileged([{ scheme: "rrs", privileges: schemeConfig }]);

let mainWindow;

app.whenReady().then(() => {
  let config = new ConfigWindow();
  mainWindow = new BrowserWindow(config);
  mainWindow.loadURL("http://localhost:3000/");
  mainWindow.webContents.openDevTools();
  CommonWindowEvent.listen();
  CommonWindowEvent.windowMaximizeChange(mainWindow);

  protocol.registerHttpProtocol("rrs", async (request, callback) => {
    if (request.method === "POST") {
      for (let i = 0; i < request.uploadData.length; i++) {
        console.log(request.uploadData[i]);
        if (request.uploadData[i].blobUUID) {
          let data = await session.defaultSession.getBlobData(request.uploadData[i].blobUUID);
          console.log(data);
        }
      }
    }
  });
});
