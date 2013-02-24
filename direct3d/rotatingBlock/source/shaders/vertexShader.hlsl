cbuffer ConstantBuffer : register( b0 ) {
	matrix rotation;
	matrix view;
	matrix projection;
	matrix translation;
}

struct VOut {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader( float4 position : POSITION, float4 color : COLOR ) {
   
	VOut output;
	output.position = mul( position, rotation );
    output.position = mul( output.position, translation );
    output.position = mul( output.position, view );
	output.position = mul( output.position, projection );
	output.color = color;
    return output;

}