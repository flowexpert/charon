<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"
		doctype-public="-//W3C//DTD HTML 4.01//EN"
		doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="basedir" select="'.'"/>
	<xsl:param name="stylesheet" select="'styles.css'"/>
	<xsl:param name="index" select="0"/>

	<xsl:template match="/">
		<html>
			<head>
				<xsl:choose>
					<xsl:when test="$index=0">
						<title>Module Group Index</title>
					</xsl:when>
					<xsl:otherwise>
						<title>Module Index</title>
					</xsl:otherwise>
				</xsl:choose>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href"><xsl:value-of select="$stylesheet"/></xsl:attribute>
				</link>
			</head>
			<body>
				<xsl:apply-templates select="doxygen/compounddef"/>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="compounddef[@kind='group']">
		<xsl:choose>
			<xsl:when test="$index=0">
				<h1><xsl:value-of select="title"/> Index</h1>
				<p><xsl:apply-templates select="briefdescription"/></p>
				<p><xsl:apply-templates select="detaileddescription"/></p>
			</xsl:when>
			<xsl:otherwise>
				<h1>Module Index</h1>
			</xsl:otherwise>
		</xsl:choose>
		<table class="index">
			<xsl:apply-templates select="innerclass"/>
		</table>
	</xsl:template>

	<xsl:template match="innerclass">
		<xsl:apply-templates select="document(concat($basedir,'/',@refid,'.xml'))/doxygen/*"/>
	</xsl:template>

	<xsl:template match="compounddef[@kind='class']">
		<tr>
			<td class="firstrow dtype">
				<a class="baseclass">
					<xsl:attribute name="href"><xsl:value-of select="@id" />.html</xsl:attribute>
					<xsl:value-of select="compoundname"/>
				</a>
			</td>
			<td class="firstrow">
				<xsl:apply-templates select="briefdescription"/>
			</td>
		</tr>
	</xsl:template>
</xsl:stylesheet>
