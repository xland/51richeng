<script lang="ts">
  import { onMount } from "svelte";

  import Alert from "./Dialog/Alert.svelte";
  import Cycle from "./Dialog/Cycle.svelte";
  import Delete from "./Dialog/Delete.svelte";
  import Donate from "./Dialog/Donate.svelte";
  import Schedule from "./Dialog/Schedule.svelte";

  let component;
  onMount(() => {
    let { ipcRenderer } = require("electron");
    ipcRenderer.once("initDialogView", (e, name: string) => {
      if (name === "Alert") component = Alert;
      else if (name === "Cycle") component = Cycle;
      else if (name === "Delete") component = Delete;
      else if (name === "Donate") component = Donate;
      else if (name === "Schedule") component = Schedule;
    });
  });
</script>

<svelte:component this={component} />

<style lang="scss"></style>
