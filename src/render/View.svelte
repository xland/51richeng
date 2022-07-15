<script lang="ts">
  import type { SettingModel } from "../model/SettingModel";

  import { onMount, tick } from "svelte";

  import "./assets/iconfont.css";
  import "./assets/main.scss";
  import { settingStore } from "./Store/SettingStore";
  import TopBar from "./TopBar.svelte";
  import Day from "./View/Day.svelte";
  import Month from "./View/Month.svelte";
  import Week from "./View/Week.svelte";
  import Year from "./View/Year.svelte";
  import Setting from "./View/Setting.svelte";
  import List from "./View/List.svelte";
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
    else if (componentName === "Setting") component = Setting;
    else if (componentName === "List") component = List;
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

<svelte:component this={component} />
