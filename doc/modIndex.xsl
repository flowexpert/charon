<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="common.xsl"/>

	<!-- Parameters that may be passed to this stylesheet -->
	<xsl:param name="basedir" select="'.'"/>

	<xsl:template match="/">
		<html>
			<head>
				<xsl:call-template name="common-head" />
				<title>Class Index</title>
			</head>
			<body>
				<h1>Class Index</h1>
				<table class="index">
					<xsl:apply-templates select="doxygenindex/compound[@kind='class']"/>
				</table>
				<xsl:call-template name="footer" />
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
