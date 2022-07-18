<script lang="ts">
  import "./assets/iconfont.css";
  import "./assets/main.scss";
  import { onMount } from "svelte";
  import Dialog from "./Dialog.svelte";
  import View from "./View.svelte";
  let component;
  onMount(() => {
    let url = new URL(document.location.href);
    let p = url.searchParams.get("viewName");
    if (p === "dialog") component = Dialog;
    else component = View;
  });
</script>

<svelte:component this={component} />
