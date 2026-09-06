const W = 250, H = 140;

Module.onRuntimeInitialized = () => {
    Module._init(W, H);
    const w = Module._get_width();
    const h = Module._get_height();
    console.log("Actual size: ", w, h);
    
    document.getElementById('status').textContent = 'ready';

    const cv = document.getElementById('canvas');
    cv.width = w; cv.height = h;
    const ctx = cv.getContext('2d');
    
    // Read pixel buffer straight out of WASM memory
    function blit() {
        const ptr  = Module._get_pixels();
        const data = new Uint8ClampedArray(Module.HEAPU8.buffer, ptr, w * h * 4);
        ctx.putImageData(new ImageData(data, w, h), 0, 0);
    }

    function render() {
        document.getElementById('status').textContent = 'rendering...';
        setTimeout(() => {
            const t0 = performance.now();
            Module._render_frame();
            blit();
            const ms = (performance.now() - t0).toFixed(0);  // roind to 0
            document.getElementById('status').textContent = `rendered in ${ms} ms`;
        }, 0);
    }

    document.getElementById('go').onclick = render;
    document.getElementById('spin').oninput = e => {
        const v = parseFloat(e.target.value);
        document.getElementById('spinval').textContent = v.toFixed(2);
        Module._set_spin(v);
    }

    // Mouse movements for camera controls
    let camDist = 50, camTheta = 57, camPhi = 180; // degrees
    let dragging = false, lastX = 0, lastY = 0;

    /* Clicks */
    cv.addEventListener('mousedown', e => {
        dragging = true; lastX = e.clientX; lastY = e.clientY;
    });
    window.addEventListener('mouseup', () => {
        if (dragging) { dragging = false; render(); }   // render on click release
    });

    /* Movement */
    window.addEventListener('mousemove', e => {
        if (!dragging) return;

        // Compute change in position
        camPhi   += (e.clientX - lastX) * 0.25;  // 0.5 sensitivity
        camTheta -= (e.clientY - lastY) * 0.25;  // inverted y axis
        camTheta  = Math.max(1, Math.min(179, camTheta));   // avoid the poles
        lastX = e.clientX; lastY = e.clientY;
        Module._set_camera(camDist, camTheta, camPhi);
    });

    // zoom
    cv.addEventListener('wheel', e => {
        e.preventDefault();
        camDist *= (e.deltaY > 0 ? 1.1 : 0.9);      // multiplicative zoom
        camDist = Math.max(10, Math.min(70, camDist));
        // console.log("camDist:", camDist);
        Module._set_camera(camDist, camTheta, camPhi);
        render();
    });

    Module._set_camera(camDist, camTheta, camPhi)
    render();
};



