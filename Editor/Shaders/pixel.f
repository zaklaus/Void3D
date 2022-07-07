
#beginfragment includes
// vector(1, 1, 1, 0)
;#define CP_1110 c0.zzzx

// vector(0, 0, 0, 1)
;#define CP_0001 c0.xxxz

#define CP_ZERO c0
#define CP_ONE c1
#define CP_FACTOR c2
#define CP_FACTOR1 c3
#define CP_COLOR c4
#define CP_COLOR1 c5
#define CP_COLOR2 c6

// value used for debugging
#define CP_DEBUG c7
#endfragment

ps.1.3

;----------------------------
#beginfragment tex_1
tex t1
#endfragment

#beginfragment tex_0
tex t0
#endfragment

#beginfragment tex_2
tex t2
#endfragment

#beginfragment tex_3
tex t3
#endfragment

//----------------------------

#beginfragment tex_1_bem
texbem t1, t0
#endfragment

#beginfragment tex_2_bem
texbem t2, t1
#endfragment

#beginfragment tex_3_bem
texbem t3, t2
#endfragment

//----------------------------

#beginfragment tex_1_beml
texbeml t1, t0
#endfragment

#beginfragment tex_2_beml
texbeml t2, t1
#endfragment

#beginfragment tex_3_beml
texbeml t3, t2
#endfragment

;----------------------------
#beginfragment texkill_0
texkill t0
#endfragment

;----------------------------
#beginfragment texkill_1
texkill t1
#endfragment

;----------------------------
#beginfragment texkill_2
texkill t2
#endfragment

;----------------------------
#beginfragment texkill_3
texkill t3
#endfragment

;----------------------------
; Simple copy of diffuse color.
#beginfragment v0_copy
mov r0, v0
#endfragment

;----------------------------
; Simple copy of diffuse color.
#beginfragment v0_mul2x
mov_x2 r0, v0
#endfragment

;----------------------------
; Simple copy of texture.
#beginfragment t0_copy
mov r0, t0
#endfragment

;----------------------------
; Copy of texture.
#beginfragment t0_copy_inv
mov r0, 1-t0
#endfragment

;----------------------------
; Simple copy of texture.
#beginfragment t1_copy
mov r0, t1
#endfragment

;----------------------------
; Modulation in stage 0.
#beginfragment mod_t0_v0
mul r0, v0, t0
#endfragment

;----------------------------
; Modulation with t1
#beginfragment mod_r0_t1
mul r0, r0, t1
#endfragment

;----------------------------
; Modulation in stage 0.
#beginfragment modx2_t0_v0
mul_x2 r0.rgb, v0, t0
+mul r0.a, v0.a, t0.a

#endfragment

;----------------------------
; Simple copy of constant color
#beginfragment color_copy
mov r0, CP_COLOR
#endfragment

;----------------------------
; Simple copy of constant color
#beginfragment mod_color_v0
mul r0, CP_COLOR, v0
#endfragment

;----------------------------
; Copy of black color
#beginfragment copy_black
mov r0.rgb, CP_ZERO
mov r0.a, CP_ONE.a
#endfragment

;----------------------------
; Copy of black color with alpha from t0
#beginfragment copy_black_t0a
mov r0.rgb, CP_ZERO
+mov r0.a, t0.a
#endfragment

;----------------------------
#beginfragment mod_t0_constcolor
mul r0, t0, CP_COLOR
#endfragment

;----------------------------
#beginfragment mod_r0_t1
mul r0, r0, t1
#endfragment

;----------------------------
#beginfragment lrp_r0_t0
lrp r1, CP_COLOR1.a, t0, CP_COLOR2
mul_x2 r0.rgb, r0, r1
+mul r0.a, r0.a, t0.a
#endfragment

;----------------------------
#beginfragment lrp_r0_t1
lrp r1, CP_COLOR1.a, t1, CP_COLOR2
mul_x2 r0.rgb, r0, r1
+mul r0.a, r0.a, t1.a
#endfragment

;----------------------------
#beginfragment lrp_r0_t2
lrp r1, CP_COLOR1.a, t2, CP_COLOR2
mul_x2 r0.rgb, r0, r1
+mul r0.a, r0.a, t2.a
#endfragment

;----------------------------
#beginfragment lrp_r0_t3
lrp r1, CP_COLOR1.a, t3, CP_COLOR2
mul_x2 r0.rgb, r0, r1
+mul r0.a, r0.a, t3.a
#endfragment

;----------------------------
#beginfragment grayscale
dp3 r_gray, r0, CP_FACTOR
lrp r0, CP_FACTOR.a, r0, r_gray
#endfragment

;----------------------------
#beginfragment modx2_r0_t0
mul_x2 r0.rgb, r0, t0
+mul r0.a, r0.a, t0.a
#endfragment

;----------------------------
#beginfragment modx2_r0_t1
mul_x2 r0.rgb, r0, t1
+mul r0.a, r0.a, t1.a
#endfragment

;----------------------------
#beginfragment modx2_r0_t2
mul_x2 r0.rgb, r0, t2
+mul r0.a, r0.a, t2.a
#endfragment

;----------------------------
#beginfragment modx2_r0_t3
mul_x2 r0.rgb, r0, t3
+mul r0.a, r0.a, t3.a
#endfragment

;----------------------------
#beginfragment modx2_t0_t1
mul_x2 r0.rgb, t0, t1
+mul r0.a, t0.a, t1.a
#endfragment

;----------------------------
#beginfragment modx2_r0_r1
mul_x2 r0.rgb, r0, r1
+mul r0.a, r0.a, r1.a
#endfragment

;----------------------------
#beginfragment modx2_t0_r1
mul_x2 r0.rgb, t0, r1
+mul r0.a, t0.a, r1.a
#endfragment

;----------------------------
#beginfragment add_t0_v0
add r0.rgb, t0, v0
mov r0.a, v0.a
#endfragment

;----------------------------
#beginfragment add_t1_v0
add r0, t1, v0
#endfragment

;----------------------------
; Shadow cast fragment.
#beginfragment shadow_cast

mov r0.rgb, CP_COLOR
+mov r0.a, t0.a

#endfragment

;----------------------------
; Shadow receive fragment.
#beginfragment shadow_receive

mov r0.rgb, t0
+mov r0.a, v0.a

#endfragment


;----------------------------
; Copy r0 blue to alpha.
#beginfragment r0_b_2_a

mov r0.a, r0.b

#endfragment


;----------------------------

#beginfragment blend_by_alpha
                              //modulate t1 and t2 using t0.a
mul r1, t1, 1-t0.a
mad r1, t2, t0.a, r1

#endfragment


//----------------------------
#beginfragment night_view

dp3 r0, t0, CP_COLOR1                      ; put to grayscale
;mad r1, r0, CP_FACTOR1, CP_FACTOR   ; select green or white, based on brightness
mul r0, 1-r0, 1-r0                  ; increase curve by multiplying 1-inversed values together few times
mul r0, r0, r0
mad r1, 1-r0, CP_FACTOR1, CP_FACTOR ; select green or white, based on brightness
;mul r0, r0, r0
mul r0, r0, r0
mul r0, r0, r0

; multiply by selected green-white color, and add some base color
mad r0, 1-r0, r1, CP_COLOR

#endfragment

#beginfragment tonemap_linear

add r0, t0, c0  ; (y = x)
add r0, r0, c7

#endfragment

#beginfragment tonemap_aces

; https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
; saturate((x*(a*x+b))/(x*(c*x+d)+e))

; Prepare left side of rational
mad r0, c2, t0, c3 ; (y1 = a*x+b)
mul r0, r0, t0     ; (y1 = x*y1)

; Prepare right side of rational
mad r1, c4, t0, c5 ; (y2 = c*x+d)
mul r1, r1, t0     ; (y2 = x*y2)
add r1, r1, c6 ; (y2 = y2+e)
mov_d2 r1, r1 ; (y2 = y2/2)

; Divide y1/y2
mul r0, r0, r1     ; (y1 = saturate(y1*y2))

; Raise gamma
add r0, r0, c7
#endfragment

#beginfragment tonemap_reinhard

; (y / (1.0f + y)

add_d2 r1, t0, CP_ONE  ; (y2 = 1/(1.0 + x))
mul r0, t0, r1         ; (y1 = x + y2)

; Raise gamma
add r0, r0, c7

#endfragment

#beginfragment colorgrade

mov_x4 r1, c3
mul r0, t0, c2
mul r0, r0, r1  ; (y = y * c)

#endfragment

;----------------------------
; Test fragment.
#beginfragment ps_test_bump

dp3_sat r1, t1_bx2, v1_bx2
add r1, r1, v0
mul_x2 r0, r0, r1

;mov r0, t1.a


#endfragment

;----------------------------
; Test fragment.
#beginfragment ps_test

nop

#endfragment
