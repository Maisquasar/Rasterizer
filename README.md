# Rasterizer
In this project we have created a programming interface.

## Summary
<ul>
    <li><a href = "#keys"> Keys </a></li>
    <li><a href = "#version"> Various Version </a></li>
    <li><a href = "#ui"> Ui </a></li>
    <li><a href = "#gallery"> Gallery </a></li>
    <li><a href = "#credits"> Credits </a></li>
</ul>

## Keys
<div id = "keys">
<li><b>W A S D</b> : Move.
<li><b>Mouse</b> : Rotate Camera.
<li><b>E</b> : Fly up.
<li><b>Q</b> : Fly down.
<li><b>Left Shift</b> : go faster.
<li><b>F1</b> : Enable/Disable WireFrame.
<li><b>F2</b> : Enable/Disable MSAA filtering.
</div>
<br><br>

## Various versions
<div id = "version">
<ul>
    <li><b>Version 1</b>: <br>
    A triangle is displayed, with a color interpolated between the 3 points (RGB).
    <li><b>Version 2</b>: <br>
    A red Cube and a blue sphere are displayed to show the z-buffer.
    <li><b>Version 3</b>: <br>
    A red Cube and a blue sphere are displayed to show the lighting under the Phong model.
    <li><b>Version 4</b>: <br>
    A red Cube and a blue sphere are displayed to show the illumination under the Blinn-Phong model in per-pixels. The Cube and the Sphere are rotated in order to show the correct calculation of the normals.
    <li><b>Version 5</b>: <br>
    Show the projection by rendering the precedent Cube and Sphere.
    <li><b>Version 6</b>: <br>
    A Cube Textured is displayed to show the texturing of the triangles.
    <li><b>Version 7</b>: <br>
    Activate the Wireframe on the previous cube. It can be disabled with the F1 key.
    <li><b>Version 8</b>: <br>
    Makes a large cube opaque, and another smaller cube 50% transparent that rotates on itself.
    <li><b>Version 9</b>: <br>
    Renders a 3d model of Mario to show everything we've done before.
    <li><b>Version 10</b>: <br>
    Render a scene to show the anti-aliasing
    <li><b>In all versions</b>: <br>
    <ul>
        <li>All models/meshs can be displayed, change position, rotation, scale, texture, color, shader and bilinear filtering.
        <li>The MSAA value can be change.
        <li>Perspective correction can be disabled/enabled.
        <li>The wireframe can be disabled/enabled.
        <li>Orthographic vision can be disabled/enabled.
        <li>Can add Lights/Show Lights.
        </ul>
</ul>
</div>
<br><br>

## Ui

<div id = "ui">
<p> Possiblity to change scale of every windows. </p>
    In the interface, the tab:
<ul>
    <li><b> Scene</b> : allows you to add lights, delete them, change the ambient, diffuse and specular parameters. The lights can have different colors and different luminosity. You can also show light.</li>
    <li><b> Config</b> : allows to display the wireFrame render, the orthographic vision, the perspective correction, the light in per pixels and you can change the MSAA value. Also displays the number of FPS. And allow you to   change the color of the background. You can also lock the scale horizontaly, verticaly and in 4/3. You can also change the filter with the slider (default : No filter). There is more than 10 filters (see in <a href = "#filter"> gallery </a>)</li>
    <li><b> Triangles</b> : Displays the number of triangles in total, and the number of triangles drawn. Also displays the number of triangles per models, When you click on a models buttons you can: see the name of the mesh, change the position, the rotation, the scale of the models, the color, the shader, and the texture. Can also choose to show or not the meshs and enable or not the bilinear filtering of every Models. There is 4 shaders (see in <a href = "#shader"> gallery </a>)
    <li> <b>Versions</b> : Allows you to change versions.</li>
    <li> <b>Framebuffer</b> : Displays the scene.</li>
</ul>
</div>

### Gallery
<div id = "gallery">
<ul>
    <li><div id = "filter">
        <details open>
            <summary>
                14 filters :
            </summary>
            <ol><b>
                <li>No filter<br></li>
                <li>Inverted:<br></li>
                    <img src="assets/ScreenShots_Gifs/Inverted.png" width="" height="500">
                <li>Red Only:<br></li>
                    <img src="assets/ScreenShots_Gifs/Red.png" width="" height="500">
                <li>Green Only:<br>
                    <img src="assets/ScreenShots_Gifs/Green.png" width="" height="500"></li>
                <li>Blue Only:<br>
                    <img src="assets/ScreenShots_Gifs/Blue.png" width="" height="500"></li>
                <li>Pixelizer:<br>
                    <img src="assets/ScreenShots_Gifs/Pixelizer.png" width="" height="500"></li>
                <li>Posterize:<br>
                    <img src="assets/ScreenShots_Gifs/Posterize.png" width="" height="500"></li>
                <li>GBC (from GameBoy Color):<br>
                    <img src="assets/ScreenShots_Gifs/GBC.png" width="" height="500"></li>
                <li>UpsideDown:<br>
                    <img src="assets/ScreenShots_Gifs/UpsideDown.png" width="" height="500"></li>
                <li>GrayScale:<br>
                    <img src="assets/ScreenShots_Gifs/GrayScale.png" width="" height="500"></li>
                <li>Hash:<br>
                    <img src="assets/ScreenShots_Gifs/Hash.png" width="" height="500"></li>
                <li>VHS:<br>
                    <img src="assets/ScreenShots_Gifs/vhs.gif" width="" height="500"></li>
                <li>Pumpkin view (source: Minecraft) </li>
                <li>Creeper view (source: Minecraft) </li>
            </b></ol>
        </details>
    </div></li>
    <li> <div id = "shader">
        <details open>
            <summary>
                4 shaders:  
            </summary>
            <ul>
                <li>The Default: <br>
                    <img src="assets/ScreenShots_Gifs/Default_Shader_ScreenShots.png" width="" height="250"></li>
                <li>The Wave: <br>
                    <img src="assets/ScreenShots_Gifs/wave.gif" width="" height="250"></li>
                <li>The Cel: <br>
                    <img src="assets/ScreenShots_Gifs/cel.gif" width="" height="250"></li>
                <li>The Glitch: <br>
                    <img src="assets/ScreenShots_Gifs/glitch.gif" width="" height="250"></li>
            </ul>
        </details>
    </div></li>
</div>

### Credits
<div if = "credits">
<li><u><b> <a href="https://gitlabstudents.isartintra.com/q.lepine">Lepine Quentin
<li><a href="https://gitlabstudents.isartintra.com/r.bourgogne"> Bourgogne Romain  </u> </b>
</div>