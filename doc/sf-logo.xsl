<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:param name="sf-logo" select="0"/>
	<xsl:template name="add-sf-logo">
		<xsl:if test="$sf-logo=1">
			<div style="text-align:center;vertical-align:middle">
				<a href="http://sourceforge.net/projects/charon-suite"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=274720&amp;type=10" width="80" height="15" alt="Get Charon at SourceForge.net. Fast, secure and Free Open Source software downloads" /></a>
			</div>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
