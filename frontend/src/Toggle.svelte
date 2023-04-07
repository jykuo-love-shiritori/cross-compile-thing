<script lang="ts">
    import LEDStore from './LEDStore';

    let selection = Array.from({ length: 4 }, (_, i) => {
        return { index: i, selected: false };
    });

    let toggle = true;

    const handleSubmit = async () => {
        $LEDStore = selection.map(({ index, selected }) => {
            return {
                index,
                enable: selected ? toggle : !toggle
            };
        });

        await fetch('http://192.168.55.1:8080/toggle', {
            method: 'POST',
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ led: $LEDStore.map((v) => v.enable) })
        });
    };
</script>

<div class="flex flex-col items-center">
    <h1 class="pb-5 text-3xl">LED Control Panel</h1>

    <div class="flex w-full flex-col pb-5 pl-8">
        {#each selection as { index, selected: enable }}
            <label>
                <input
                    type="checkbox"
                    bind:checked={enable}
                    value={index}
                    class="mr-1"
                />
                LED {index}
            </label>
        {/each}
    </div>

    <div class="pb-5">
        <label class="mx-3">
            <input type="radio" bind:group={toggle} value={true} />
            ON
        </label>

        <label class="mx-3">
            <input type="radio" bind:group={toggle} value={false} />
            OFF
        </label>
    </div>

    <button on:click={handleSubmit} class="rounded-full bg-blue-600 px-5 py-1">
        Submit
    </button>
</div>
