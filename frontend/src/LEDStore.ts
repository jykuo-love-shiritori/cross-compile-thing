import { writable } from 'svelte/store';

export default writable(
    Array.from({ length: 4 }, (_, i) => {
        return { index: i, enable: false };
    })
);
