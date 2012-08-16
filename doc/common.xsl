<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"
		doctype-public="-//W3C//DTD HTML 4.01//EN"
		doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="stylesheet" select="'styles.css'"/>
	<xsl:param name="sf-logo" select="0" />

	<xsl:template name="add-sf-logo">
		<xsl:if test="$sf-logo=1">
			<div style="text-align:center;vertical-align:middle">
				<hr />
				<a href="http://sourceforge.net/projects/charon-suite">
					<img src="http://sflogo.sourceforge.net/sflogo.php?group_id=274720&amp;type=10" width="80" height="15" alt="Get Charon at SourceForge.net. Fast, secure and Free Open Source software downloads" />
				</a>
			</div>
		</xsl:if>
	</xsl:template>

	<xsl:variable name="title" />
	<xsl:template name="header" />
	<xsl:template name="content" />
	<xsl:template name="footer">
		<xsl:call-template name="add-sf-logo" />
	</xsl:template>

	<xsl:template match="/">
		<html>
			<head>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href"><xsl:value-of select="$stylesheet"/></xsl:attribute>
				</link>
				<title><xsl:value-of select="$title" /></title>
			</head>
			<body>
				<xsl:call-template name="header" />
				<xsl:call-template name="content" />
				<xsl:call-template name="footer" />
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
