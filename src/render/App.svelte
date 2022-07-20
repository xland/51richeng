<script lang="ts">
  import "./assets/iconfont.css";
  import "./assets/main.scss";
  import { onMount } from "svelte";
  import Alert from "./Dialog/Alert.svelte";
  import Cycle from "./Dialog/Cycle.svelte";
  import Delete from "./Dialog/Delete.svelte";
  import Donate from "./Dialog/Donate.svelte";
  import Schedule from "./Dialog/Schedule.svelte";
  import View from "./View.svelte";
  let component;
  onMount(() => {
    let url = new URL(document.location.href);
    let name = url.searchParams.get("dialog");
    if (name === "Alert") component = Alert;
    else if (name === "Cycle") component = Cycle;
    else if (name === "Delete") component = Delete;
    else if (name === "Donate") component = Donate;
    else if (name === "Schedule") component = Schedule;
    else component = View;
  });
</script>

<svelte:component this={component} />
