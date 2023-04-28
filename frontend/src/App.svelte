<script lang="ts">
    import Bling from './Bling.svelte';
    import Kjy from './KJY.svelte';
    import Stream from './Stream.svelte';
    import Toggle from './Toggle.svelte';
</script>

<div
    class="flex h-screen flex-col items-center overflow-clip bg-neutral-800 p-3 text-gray-200"
>
    <Kjy />
    <hr class="my-4 h-px w-1/3 border-0 bg-gray-200" />

    <Stream />

    <hr class="my-4 h-px w-1/3 border-0 bg-gray-200" />

    <Toggle />

    <hr class="my-4 h-px w-1/3 border-0 bg-gray-200" />

    <Bling />
</div>
