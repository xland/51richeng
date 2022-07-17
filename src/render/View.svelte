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
  let viewName: string;
  let subscribe = () => {
    settingStore.subscribe((v: SettingModel) => {
      setting = v;
      initView();
    });
  };
  let initView = async () => {
    if (setting.viewSelected === "lastSelected") {
      viewName = setting.viewLastSelected;
    } else {
      viewName = setting.viewSelected;
    }
    if (viewName === "Month") component = Month;
    else if (viewName === "Day") component = Day;
    else if (viewName === "Week") component = Week;
    else if (viewName === "Year") component = Year;
    else if (viewName === "Setting") component = Setting;
    else if (viewName === "List") component = List;
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
  <TopBar {viewName} />
{/if}

<svelte:component this={component} />
