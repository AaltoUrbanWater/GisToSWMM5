<!DOCTYPE qgis PUBLIC 'http://mrcc.com/qgis.dtd' 'SYSTEM'>
<qgis version="2.18.11" minimumScale="inf" maximumScale="1e+08" hasScaleBasedVisibilityFlag="0">
  <pipe>
    <rasterrenderer opacity="1" alphaBand="-1" classificationMax="62" classificationMinMaxOrigin="MinMaxFullExtentEstimated" band="1" classificationMin="10" type="singlebandpseudocolor">
      <rasterTransparency/>
      <rastershader>
        <colorrampshader colorRampType="EXACT" clip="0">
          <item alpha="255" value="10" label="Connected roof" color="#e97a7a"/>
          <item alpha="255" value="20" label="Unconnected roof" color="#e9e9e9"/>
          <item alpha="255" value="30" label="Sand and gravel" color="#eed8a6"/>
          <item alpha="255" value="31" label="Asphalt" color="#3a3a3a"/>
          <item alpha="255" value="32" label="Stone paver" color="#aaaaaa"/>
          <item alpha="255" value="60" label="Rock outcrop" color="#9092b0"/>
          <item alpha="255" value="61" label="Vegetation" color="#c6e471"/>
          <item alpha="255" value="62" label="Water" color="#a4ebff"/>
        </colorrampshader>
      </rastershader>
    </rasterrenderer>
    <brightnesscontrast brightness="0" contrast="0"/>
    <huesaturation colorizeGreen="128" colorizeOn="0" colorizeRed="255" colorizeBlue="128" grayscaleMode="0" saturation="0" colorizeStrength="100"/>
    <rasterresampler maxOversampling="2"/>
  </pipe>
  <blendMode>0</blendMode>
</qgis>
