infixl 6 :+:
infixl 7 :*:
data Expr = Val Int | Expr :+: Expr | Expr :*: Expr
    deriving (Show, Eq)

expand :: Expr -> Expr
expand (Val a) = (Val a)
expand ((Val a) :*: (Val b)) = (Val a) :*: (Val b)
expand (e1 :+: e2) = expand e1 :+: expand e2
expand ((e1 :+: e2) :*: e) = expand ( e1 :*: e ) :+: expand ( e2 :*: e )
expand (e :*: (e1 :+: e2)) = expand ( e :*: e1 ) :+: expand ( e :*: e2 )
expand (e1 :*: e2) = expand' (e1 :*: e2) q
			where q = expand e1 :*: expand e2
			      expand' act q = if q == act then q
					else expand' q (expand q)
--(Val 1 :+: Val 2 :+: Val 5) :*: ( Val 3 :+: Val 7) :*: Val 11


