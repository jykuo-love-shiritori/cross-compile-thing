<script lang="ts">
    import KJY from './assets/kjyLight.png';
    import LEDStore from './LEDStore';
</script>

<div class="flex">
    {#each $LEDStore as { index, enable }}
        <div class="flex flex-col items-center">
            <div class="m-5 h-20 w-20">
                {#if enable}
                    <img
                        src={KJY}
                        alt=""
                        class="h-full w-full object-contain"
                    />
                {/if}
            </div>

            <span>LED {index}</span>
        </div>
    {/each}
</div>
