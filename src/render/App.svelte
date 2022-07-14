<script lang="ts">
  import type { SettingModel } from "../model/SettingModel";

  import { onMount, tick } from "svelte";

  import "./assets/iconfont.css";
  import "./assets/main.scss";
  import ContextMenu from "./ContextMenu.svelte";
  import { settingStore } from "./Store/SettingStore";
  import TopBar from "./TopBar.svelte";
  import Day from "./View/Day.svelte";
  import Month from "./View/Month.svelte";
  import Week from "./View/Week.svelte";
  import Year from "./View/Year.svelte";
  let setting: SettingModel;
  let component;
  let subscribe = () => {
    settingStore.subscribe((v: SettingModel) => {
      setting = v;
      initView();
    });
  };
  let initView = async () => {
    let componentName = setting.viewSelected;
    if (setting.viewSelected === "lastSelected") {
      componentName = setting.viewLastSelected;
    }
    if (componentName === "Month") component = Month;
    else if (componentName === "Day") component = Day;
    else if (componentName === "Week") component = Week;
    else if (componentName === "Year") component = Year;
    await tick();
    let { ipcRenderer } = require("electron");
    ipcRenderer.invoke("pinWindow");
  };
  onMount(async () => {
    await settingStore.init();
    subscribe();
  });
</script>

{#if component}
  <TopBar />
{/if}

<div id="viewBox">
  <svelte:component this={component} />
</div>

<ContextMenu />

<style lang="scss">
  #viewBox {
    position: absolute;
    top: 46px;
    bottom: 0px;
    left: 0px;
    right: 0px;
  }
</style>
