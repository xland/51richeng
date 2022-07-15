<script lang="ts">
  import { onMount } from "svelte";
  import Dialog from "./Dialog.svelte";
  import View from "./View.svelte";
  let component;
  onMount(() => {
    let url = new URL(document.location.href);
    let p = url.searchParams.get("p");
    if (p === "dialog") component = Dialog;
    else component = View;
  });
</script>

<svelte:component this={component} />
