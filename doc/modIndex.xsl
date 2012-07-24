<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="sf-logo.xsl"/>
	<xsl:output method="html" encoding="UTF-8"
		doctype-public="-//W3C//DTD HTML 4.01//EN"
		doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="basedir" select="'.'"/>
	<xsl:param name="stylesheet" select="'styles.css'"/>

	<xsl:template match="/">
		<html>
			<head>
				<title>Class Index</title>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href"><xsl:value-of select="$stylesheet"/></xsl:attribute>
				</link>
			</head>
			<body>
				<h1>Class Index</h1>
				<table class="index">
					<xsl:apply-templates select="doxygenindex/compound[@kind='class']"/>
				</table>
				<xsl:call-template name="add-sf-logo"/>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="compound">
		<xsl:if test="not(starts-with(@refid,'classstd'))">
			<tr>
				<td class="firstrow dtype">
					<a class="baseclass">
						<xsl:attribute name="href"><xsl:value-of select="@refid" />.html</xsl:attribute>
						<xsl:value-of select="name"/>
					</a>
				</td>
				<td class="firstrow">
					<xsl:apply-templates select="document(concat($basedir,'/',@refid,'.xml'))/doxygen/*"/>
				</td>
			</tr>
		</xsl:if>
	</xsl:template>

	<xsl:template match="compounddef">
		<xsl:apply-templates select="briefdescription"/>
	</xsl:template>
</xsl:stylesheet>
