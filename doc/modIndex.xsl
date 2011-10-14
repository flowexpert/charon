<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8" doctype-public="-//W3C//DTD HTML 4.01//EN" doctype-system="http://www.w3.org/TR/html4/strict.dtd" />
	<xsl:template match="/">
		<html>
			<head>
				<title>Module Index</title>
				<link rel="stylesheet" type="text/css" href="styles.css" />
			</head>
			<body>
				<h1>Module Index</h1>
				<table class="index">
					<xsl:apply-templates select="doxygenindex/compound"/>
				</table>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="compound">
		<tr>
			<td class="firstrow dtype">
				<a class="baseclass">
					<xsl:attribute name="href"><xsl:value-of select="@refid" />.html</xsl:attribute>
					<xsl:value-of select="name"/>
				</a>
			</td>
			<td class="firstrow">
				<xsl:apply-templates select="document( concat( @refid, '.xml' ) )/doxygen/*" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="compounddef">
		<xsl:apply-templates select="briefdescription" />
	</xsl:template>
</xsl:stylesheet>
