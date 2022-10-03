<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
    version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:exi="https://example.org/input"
    xmlns:exo="https://exmaple.org/output"
    >
  <xsl:output encoding="UTF-8"/>

  <xsl:template match="/exi:input">
    <exo:output/>
  </xsl:template>
</xsl:stylesheet>
