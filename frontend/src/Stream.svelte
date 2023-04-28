<script lang="ts">
    let stream = false;

    let interval: NodeJS.Timer | undefined;

    let value = 0;

    const handleSubmit = async () => {
        if (stream) {
            if (!interval) {
                interval = setInterval(fetchData, 1000);
            }
        } else {
            clearInterval(interval);
            interval = undefined;
        }
    };

    const fetchData = async () => {
        const res = await fetch('http://192.168.55.1:8080/stream');
        const data = await res.json();

        console.log(data);
        value = data.value;
    };
</script>

<div class="flex flex-col items-center">
    <h1 class="mb-5 text-3xl">Stream</h1>

    <div class="pb-5">
        <label class="mx-3">
            <input type="radio" bind:group={stream} value={true} />
            ON
        </label>

        <label class="mx-3">
            <input type="radio" bind:group={stream} value={false} />
            OFF
        </label>
    </div>

    <div class="pb-5">
        {value}
    </div>

    <button on:click={handleSubmit} class="rounded-full bg-blue-600 px-5 py-1">
        Submit
    </button>
</div>
