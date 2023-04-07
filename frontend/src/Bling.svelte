<script lang="ts">
    import LEDStore from './LEDStore';

    let times = 5;
    let speed = 1;
    let speedText = 'SPEED';
    let isValid = true;

    const handleTimesUpdate = async () => {
        isValid = Number.isInteger(times) && times >= 0;
    };

    const handleSpeedUpdate = async () => {
        speedText = 'SPE' + 'E'.repeat(Math.floor(speed * 2 - 1)) + 'D';
    };

    const handleSubmit = async () => {
        fetch('http://192.168.55.1:8080/bling', {
            method: 'POST',
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                times: times,
                interval: 2000 / speed
            })
        });

        for (let i = 0; i < times; i++) {
            setTimeout(() => {
                $LEDStore[0].enable = i % 2 === 0;
                $LEDStore[1].enable = i % 2 === 0;
                $LEDStore[2].enable = i % 2 !== 0;
                $LEDStore[3].enable = i % 2 !== 0;
            }, i * (2000 / speed));
        }
    };
</script>

<div class="flex flex-col items-center">
    <h1 class="mb-5 text-3xl">LED Go Bling Bling</h1>

    {2000 / speed}

    <div class="mb-5 w-full items-center">
        <div>
            {#if isValid}
                <p>Times: {times}</p>
            {:else}
                <p class="text-red-600">Invalid input</p>
            {/if}

            <input
                type="number"
                bind:value={times}
                min="0"
                class="mb-3 w-full px-1 text-black"
                on:input={handleTimesUpdate}
            />
        </div>

        <div class="relative">
            <p class="absolute">{speedText}</p>

            <input
                type="range"
                bind:value={speed}
                min="1"
                max="100"
                class="mt-6 w-full"
                on:input={handleSpeedUpdate}
            />
        </div>
    </div>

    <button
        on:click={handleSubmit}
        class="rounded-full bg-blue-600 px-5 py-1
        transition-all disabled:bg-blue-400"
        disabled={!isValid}
    >
        Submit
    </button>
</div>
