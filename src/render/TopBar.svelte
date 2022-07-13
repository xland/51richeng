<script lang="ts">
  import { onMount } from "svelte";
  import { settingStore } from "./Store/SettingStore";

  let isMaximized = true;
  let closeWindow = () => {
    let { ipcRenderer } = require("electron");
    if (location.href.includes("/main")) {
      // ipcRenderer.invoke("hideWindow");
      //todo
      ipcRenderer.invoke("closeWindow");
    } else {
      ipcRenderer.invoke("closeWindow");
    }
  };
  let maxmizeMainWin = () => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("maxmizeWindow");
  };
  let minimizeMainWindow = () => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("minimizeWindow");
  };
  let unmaximizeMainWindow = () => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("unmaximizeWindow");
  };
  onMount(() => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.on("windowMaximized", () => {
      isMaximized = true;
      settingStore.updateMaximizeState(isMaximized, window.outerWidth, window.outerHeight);
    });
    ipcRenderer.on("windowUnmaximized", () => {
      isMaximized = false;
      settingStore.updateMaximizeState(isMaximized, window.outerWidth, window.outerHeight);
    });
  });
</script>

<div class="topBar" style="left: 300px;">
  <div class="winTitle" />
  <div class="winTool">
    <div on:click={minimizeMainWindow}>
      <i class="icon iconminimize" />
    </div>
    {#if isMaximized}
      <div on:click={unmaximizeMainWindow}>
        <i class="icon iconrestore" />
      </div>
    {:else}
      <div on:click={maxmizeMainWin}>
        <i class="icon iconmaximize" />
      </div>
    {/if}
    <div on:click={closeWindow}>
      <i class="icon iconclose" />
    </div>
  </div>
</div>

<style lang="scss">
  .topBar {
    position: absolute;
    right: 0px;
    top: 0px;
    display: flex;
    height: 30px;
    line-height: 30px;
    -webkit-app-region: drag;
    z-index: 2;
  }
  .winTitle {
    flex: 1;
    padding-left: 12px;
    font-size: 14px;
  }
  .winTool {
    height: 100%;
    display: flex;
    -webkit-app-region: no-drag;
  }
  .winTool div {
    height: 100%;
    width: 36px;
    text-align: center;
    color: #666666;
    cursor: pointer;
    line-height: 30px;
  }
  .winTool .icon {
    font-size: 12px;
    color: #666666;
  }
  .winTool div:hover {
    background: #efefef;
  }
  .winTool div:last-child:hover {
    background: #ff7875;
  }
  .winTool div:last-child:hover i {
    color: #fff !important;
  }
</style>
